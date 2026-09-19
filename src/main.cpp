#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include "secrets.h"

namespace {
constexpr uint8_t TRIG_PIN = 5;
constexpr uint8_t ECHO_PIN = 18;

// Calibrate these two values for the physical bin.
constexpr float EMPTY_DISTANCE_CM = 35.0f;
constexpr float FULL_DISTANCE_CM = 7.0f;
constexpr uint8_t FULL_THRESHOLD_PERCENT = 85;
constexpr uint8_t RESET_THRESHOLD_PERCENT = 70;

constexpr unsigned long SENSOR_INTERVAL_MS = 5000;
constexpr unsigned long WIFI_RETRY_INTERVAL_MS = 10000;
constexpr unsigned long TELEGRAM_COOLDOWN_MS = 30000;
constexpr unsigned long ECHO_TIMEOUT_US = 30000;
constexpr uint8_t SAMPLE_COUNT = 5;

WiFiClientSecure secureClient;
UniversalTelegramBot bot(BOT_TOKEN, secureClient);

unsigned long lastSensorReadMs = 0;
unsigned long lastWiFiAttemptMs = 0;
unsigned long lastNotificationMs = 0;
int lastReportedPercent = -1;
bool fullNotificationSent = false;
bool startupNotificationSent = false;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) {
    return NAN;
  }
  return static_cast<float>(duration) * 0.0343f / 2.0f;
}

float readFilteredDistanceCm() {
  float samples[SAMPLE_COUNT];
  uint8_t validCount = 0;

  for (uint8_t i = 0; i < SAMPLE_COUNT; ++i) {
    const float distance = readDistanceCm();
    if (!isnan(distance) && distance >= 2.0f && distance <= 400.0f) {
      samples[validCount++] = distance;
    }
    delay(25);
  }

  if (validCount == 0) {
    return NAN;
  }

  for (uint8_t i = 0; i + 1 < validCount; ++i) {
    for (uint8_t j = i + 1; j < validCount; ++j) {
      if (samples[j] < samples[i]) {
        const float temp = samples[i];
        samples[i] = samples[j];
        samples[j] = temp;
      }
    }
  }

  return samples[validCount / 2];
}

int distanceToPercent(const float distanceCm) {
  const float span = EMPTY_DISTANCE_CM - FULL_DISTANCE_CM;
  if (span <= 0.0f) {
    return 0;
  }

  const float rawPercent =
      ((EMPTY_DISTANCE_CM - distanceCm) / span) * 100.0f;
  return constrain(static_cast<int>(lroundf(rawPercent)), 0, 100);
}

bool canNotify() {
  return lastNotificationMs == 0 ||
         millis() - lastNotificationMs >= TELEGRAM_COOLDOWN_MS;
}

void notifyTelegram(const String& message) {
  if (WiFi.status() != WL_CONNECTED || !canNotify()) {
    return;
  }

  Serial.print("Telegram: ");
  Serial.println(message);

  if (bot.sendMessage(CHAT_ID, message, "")) {
    lastNotificationMs = millis();
  } else {
    Serial.println("Telegram notification failed");
  }
}

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED ||
      millis() - lastWiFiAttemptMs < WIFI_RETRY_INTERVAL_MS) {
    return;
  }

  lastWiFiAttemptMs = millis();
  Serial.printf("Connecting to Wi-Fi: %s\n", WIFI_SSID);
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void handleFillLevel(const float distanceCm) {
  const int fillPercent = distanceToPercent(distanceCm);
  Serial.printf("Distance: %.1f cm | Fill: %d%%\n", distanceCm, fillPercent);

  if (lastReportedPercent < 0) {
    lastReportedPercent = fillPercent;
    return;
  }

  if (fillPercent >= FULL_THRESHOLD_PERCENT && !fullNotificationSent) {
    notifyTelegram("🗑️ Dustbin is FULL (" + String(fillPercent) +
                   "%). Please empty it.");
    fullNotificationSent = true;
  } else if (fillPercent <= RESET_THRESHOLD_PERCENT && fullNotificationSent) {
    notifyTelegram("✅ Dustbin emptied. Current level: " +
                   String(fillPercent) + "%." );
    fullNotificationSent = false;
  }

  lastReportedPercent = fillPercent;
}
}  // namespace

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  WiFi.mode(WIFI_STA);
  secureClient.setInsecure();  // Replace with a root CA for production use.
  connectWiFi();
}

void loop() {
  connectWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    if (!startupNotificationSent) {
      notifyTelegram("🤖 Smart dustbin online. Monitoring fill level.");
      if (lastNotificationMs != 0) {
        startupNotificationSent = true;
      }
    }
  } else {
    startupNotificationSent = false;
  }

  if (millis() - lastSensorReadMs >= SENSOR_INTERVAL_MS) {
    lastSensorReadMs = millis();
    const float distanceCm = readFilteredDistanceCm();
    if (isnan(distanceCm)) {
      Serial.println("No valid ultrasonic reading");
    } else {
      handleFillLevel(distanceCm);
    }
  }

  delay(20);
}
