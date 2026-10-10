# Universal Smart Node 🌐

> **One modular IoT node. Many real-world applications.**

Universal Smart Node is an ESP32-based IoT platform designed to make connected sensing and automation easier to deploy across different environments. Instead of building a completely separate device for every use case, the goal is to create a reusable core that can be adapted with different sensors, rules, and notification channels.

**Project status:** Prototype / in development. Features marked as planned are part of the roadmap and may not be implemented in the current firmware yet.

---

## Contents

- [Vision](#-vision)
- [The Problem](#-the-problem)
- [Our Approach](#-our-approach)
- [Architecture](#-architecture)
- [Core Features](#-core-features)
- [Example Applications](#-example-applications)
- [Hardware](#-hardware)
- [Software and Services](#-software-and-services)
- [Getting Started](#-getting-started)
- [Configuration and Security](#-configuration-and-security)
- [Development Roadmap](#-development-roadmap)
- [Contributing](#-contributing)

## 💡 Vision

Build a reusable, affordable, and extensible IoT node that can monitor its surroundings, detect meaningful events, and notify the right person or system.

The same core device could help a cleaning team know when a bin is full, remind a caregiver about a medicine box, or monitor environmental conditions. The sensor and application logic change; the core connectivity and event-handling approach should remain reusable.

## 🧩 The Problem

Many small IoT ideas become expensive or difficult to maintain because each one needs a different firmware project, hardcoded network credentials, custom app, and separate notification workflow.

Universal Smart Node aims to reduce that friction through:

- **Reusable firmware:** a common foundation for multiple sensor-based projects.
- **Flexible connectivity:** Wi-Fi setup without baking a specific network password into the source code.
- **Event-driven alerts:** notify people when a meaningful threshold or condition is reached.
- **Modular hardware:** select sensors according to the use case.
- **Expandable integration:** leave room for APIs, a dashboard, data storage, and additional communication methods.

## 🛠️ Our Approach

The project is built around an ESP32 microcontroller. Sensors provide readings, application rules interpret them, and the node can trigger an action such as sending a notification or reporting data to a backend.

The intended design separates reusable device infrastructure from use-case-specific behaviour. For example, a smart dustbin and a medicine reminder box can share connectivity and notification modules while using different sensors and event rules.

## 🏗️ Architecture

```text
         Sensors / Inputs
                 |
                 v
          ESP32 Smart Node
        +-------------------+
        | Read sensor data  |
        | Apply event rules |
        | Check device state|
        +-------------------+
                 |
       Wi-Fi / configured link
                 |
        +--------+---------+
        |                  |
        v                  v
   Telegram alerts    HTTP / REST API
                           |
                           v
                   Backend / Dashboard
                   (planned integration)
```

**Design principle:** keep the device core reusable and move use-case-specific thresholds, sensor selection, and actions into configurable modules wherever practical.

## ✨ Core Features

### Current direction

- ESP32 as the core microcontroller.
- Sensor-based monitoring for application-specific conditions.
- A modular approach intended to support multiple projects.
- Development and testing with real hardware.

### Planned capabilities

- **Wi-Fi provisioning:** use a captive portal or Wi-Fi manager so a new installation can be configured without editing firmware credentials.
- **Event-driven notifications:** Telegram Bot API integration for actionable alerts.
- **Backend connectivity:** HTTP/REST communication with a service such as a Next.js API and Supabase.
- **Remote monitoring and control:** a web dashboard for device status and supported settings.
- **Reusable configuration:** thresholds, device identifiers, and enabled modules configured per deployment.
- **Reliability improvements:** reconnection handling, sensor error states, alert cooldowns, and health reporting.
- **Firmware updates:** explore secure over-the-air (OTA) updates as the platform matures.

These are roadmap items unless verified in the current source code. Do not assume every feature is already available in the prototype.

## 🌱 Example Applications

The platform is intended to support different sensing and monitoring scenarios. These are potential applications, not claims that each system has already been built or validated.

### Initial prototypes

1. **Smart Dustbin** — monitor fill level with an ultrasonic sensor and notify cleaning staff when the bin needs attention.
2. **Medicine Reminder Box** — use a suitable sensor and configured schedule to detect a possible missed interaction and notify a caregiver. This is a reminder aid, not a medical device.
3. **Bathroom Exhaust Monitor** — use suitable environmental sensors or switching inputs to support ventilation rules.

### Potential use cases

#### Nature, wildlife, and agriculture
4. Forest-fire risk sensing and distributed environmental monitoring.
5. Beehive acoustic and activity monitoring.
6. Termite activity detection using vibration sensing.
7. Environmental conditions associated with fungal-spore risk.
8. Tension monitoring near designated wildlife corridors, where lawful and safely installed.
9. Grain-silo temperature, humidity, and fermentation-condition monitoring.

#### Infrastructure and industry
10. Concrete curing condition monitoring.
11. Early warning for sewer blockage indicators.
12. Gym cable or equipment strain monitoring with suitable rated sensors.
13. Road-surface temperature monitoring to help identify possible black-ice conditions.
14. Landfill gas or methane monitoring with appropriate certified sensing hardware.
15. Water-tank level monitoring.
16. Pump or motor condition monitoring using suitable vibration/current sensors.

#### Buildings, heritage, and everyday spaces
17. Rare-book storage humidity and mold-risk monitoring.
18. Public-bin fill-level monitoring.
19. Room occupancy or utility-space condition monitoring using privacy-conscious sensors.
20. Cold-storage temperature monitoring.
21. Leak detection in utility areas.

Some applications, especially fire, gas, structural, medical, and wildlife-related monitoring, require suitable certified sensors, careful validation, and safety review. A hobby prototype must not be treated as a safety-critical system.

## 🔌 Hardware

The exact bill of materials depends on the application. A basic development setup may include:

| Component | Purpose |
|---|---|
| ESP32 development board | Main controller and Wi-Fi connectivity |
| Application-specific sensor | Measures fill level, temperature, humidity, vibration, or another condition |
| Stable power supply | Powers the node and connected sensors |
| Breadboard, jumper wires, or suitable PCB | Prototyping and electrical connections |
| Enclosure and mounting hardware | Protects the device in its intended environment |

For the smart-dustbin prototype, an **HC-SR04 ultrasonic sensor** can be used to estimate the distance from the sensor to the waste surface. Select a sensor and enclosure suitable for the actual environment; ordinary hobby modules may not tolerate rain, dust, condensation, or corrosive gases.

**Power and safety:** verify the voltage requirements of every module, use appropriate level shifting where needed, and do not connect mains-powered equipment directly to the ESP32.

## 💻 Software and Services

| Technology | Intended role |
|---|---|
| C/C++ with Arduino framework or ESP-IDF | ESP32 firmware |
| WiFiManager or an equivalent provisioning method | Initial Wi-Fi setup |
| Telegram Bot API | Optional notification channel |
| HTTP/REST | Communication with a backend |
| Next.js | Potential web dashboard or API layer |
| Supabase | Potential device/event data storage |
| Git and GitHub | Version control and collaboration |

The exact framework and service choices may change during development. Integrations should be enabled only after their implementation and configuration have been tested.

## 🚀 Getting Started

### 1. Prepare the development environment

- Install Arduino IDE or PlatformIO.
- Install the ESP32 board support package or configure the ESP32 platform.
- Identify the firmware entry point and required libraries in the repository.
- Connect the ESP32 over USB and select the correct board and serial port.

### 2. Connect a sensor

Wire the selected sensor according to its datasheet and the pin assignments in the firmware. Check voltage compatibility before powering the circuit. For an ultrasonic sensor such as the HC-SR04, ensure the ESP32's input pin is not exposed to an incompatible 5 V echo signal; use an appropriate divider or level shifter when required.

### 3. Configure the device

Use the configuration method implemented by the current firmware. If Wi-Fi provisioning has not yet been added, configure the development network locally for testing and keep credentials out of committed source files.

### 4. Build and flash

Open the firmware project, install its declared dependencies, select the board and port, then compile and upload. Monitor the serial output to confirm boot, connectivity, and sensor readings.

### 5. Test safely

Test sensor readings and event thresholds in a controlled environment before relying on notifications. Verify what happens during Wi-Fi loss, invalid sensor readings, restarts, and repeated threshold crossings.

> **Note:** This README describes the intended platform. Exact build commands, pin mappings, and configuration keys should be documented alongside the firmware once the current source layout and implementation are confirmed.

## 🔐 Configuration and Security

- Never commit Wi-Fi passwords, bot tokens, API keys, or backend service-role keys.
- Keep secrets in local, ignored configuration files or a suitable secret-management system.
- Use a dedicated Telegram bot and share its token only with trusted maintainers.
- Validate incoming data and restrict any remote-control endpoints.
- Add authentication and transport security before exposing device controls outside a trusted network.
- Collect only the data needed for the application and consider privacy before installing sensors in shared spaces.

## 🗺️ Development Roadmap

- [ ] Establish a stable ESP32 firmware baseline.
- [ ] Define a consistent sensor interface and per-application configuration.
- [ ] Add Wi-Fi provisioning without hardcoded deployment credentials.
- [ ] Implement robust sensor reading, filtering, and error handling.
- [ ] Add configurable event thresholds and notification cooldowns.
- [ ] Integrate Telegram alerts for the first working prototype.
- [ ] Validate the Smart Dustbin use case with real-world tests.
- [ ] Add HTTP/REST reporting and optional backend storage.
- [ ] Build a dashboard for device status and event history.
- [ ] Add device health reporting, reconnection handling, and secure OTA updates.
- [ ] Document tested hardware combinations and measured operating costs.

## 🤝 Contributing

Ideas, testing feedback, hardware notes, and improvements are welcome.

1. Open an issue describing the use case or problem.
2. Include the board, sensor model, wiring, and steps to reproduce any hardware issue.
3. Keep credentials and personal data out of commits and issue reports.
4. Submit focused changes with clear descriptions and test results.

## 📌 Project Status

Universal Smart Node is an evolving engineering project. Hardware compatibility, reliability, costs, and each application must be tested before deployment. Features in the roadmap should be treated as planned until verified in code and on hardware.

---

**Built as a step toward reusable, practical, and accessible IoT systems.**
