# 🏠 Smart Home Automation System using ESP32

![ESP32](https://img.shields.io/badge/MCU-ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/IDE-Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![IoT](https://img.shields.io/badge/Domain-IoT-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge)

A **fully functional IoT Smart Home Automation system** built using the ESP32 microcontroller — integrating 3 independent automation modules: an ultrasonic-based automatic door, an MQ-2 gas leakage detection and alert system, and an automatic water tank pump controller. All three systems run concurrently on a single ESP32 in a continuous real-time monitoring loop.

---

## 📋 Table of Contents

- [About the Project](#about-the-project)
- [System Architecture](#system-architecture)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Pin Configuration](#pin-configuration)
- [Working Logic](#working-logic)
- [Code Structure](#code-structure)
- [Libraries Used](#libraries-used)
- [Circuit Diagram](#circuit-diagram)
- [Setup & Installation](#setup--installation)
- [Future Improvements](#future-improvements)
- [Developer](#developer)

---

## 📌 About the Project

The **Smart Home Automation System** is an embedded IoT project built on the ESP32 microcontroller that automates three critical home functions — entry/exit door control, gas leak safety, and water tank management — using dedicated sensors and actuators.

The ESP32 runs a continuous polling loop that reads sensor data in real time and triggers appropriate hardware responses with no manual intervention required. All three subsystems operate independently and simultaneously on the same microcontroller.

This project was built as part of my academic work at **Bhavan's College, Mumbai** during my B.Sc. Computer Science (SYCS) program.

---

## 🏗️ System Architecture

```
                        ┌─────────────────────────┐
                        │         ESP32            │
                        │                          │
    HC-SR04 ────────────►  Ultrasonic Input        │
    Servo Motor ◄───────┤  Door Control Output     │
                        │                          │
    MQ-2 Sensor ────────►  Gas Level Input (ADC)   │
    Buzzer ◄────────────┤  Alert Output            │
                        │                          │
    Water Level ────────►  Tank Level Input        │
    Relay Module ◄──────┤  Pump Control Output     │
                        │                          │
                        └─────────────────────────┘
```

The ESP32 continuously reads all 3 sensors in the `loop()` function and independently controls their corresponding actuators based on threshold logic.

---

## ✅ Features

### 🚪 1. Automatic Door Control System

Detects approaching objects or people using an **HC-SR04 Ultrasonic Sensor** and controls a **Servo Motor** to open or close the door automatically.

**Logic:**
```
Distance ≤ 10 cm  →  Servo rotates to OPEN position  (door opens)
Distance > 15 cm  →  Servo rotates to CLOSED position (door closes)
```

- Uses the time-of-flight principle: sends ultrasonic pulse via `TRIG` pin, measures echo return time via `ECHO` pin
- Distance calculated as: `distance = (duration × 0.034) / 2`
- Servo controlled via `ESP32Servo.h` — no PWM conflict issues
- 5 cm hysteresis band (10 cm open / 15 cm close) prevents rapid door flutter

---

### 🔥 2. Gas Leakage Detection & Alert System

Continuously monitors ambient gas levels using an **MQ-2 Gas Sensor** (detects LPG, methane, smoke, propane) and triggers an audible **Buzzer** alert if dangerous levels are detected.

**Logic:**
```
Gas Level > 800 (analog threshold)  →  Buzzer ON  (alert active)
Gas Level ≤ 800                     →  Buzzer OFF (safe)
```

- MQ-2 outputs an analog voltage proportional to gas concentration
- ESP32's ADC reads the analog pin and compares against threshold value `800`
- Immediate real-time response — no delay between detection and alert
- Threshold value `800` can be calibrated in code based on sensor placement and environment

---

### 💧 3. Automatic Water Tank Management

Monitors water tank fill level using a **Water Level Sensor** and controls a **Relay Module** to automatically turn the water pump ON or OFF.

**Logic:**
```
Tank FULL  (sensor HIGH)  →  Relay OFF → Pump turns OFF (prevents overflow)
Tank EMPTY (sensor LOW)   →  Relay ON  → Pump turns ON  (refills tank)
```

- Prevents tank overflow by automatically cutting pump power when full
- Prevents tank running dry by auto-starting the pump when level drops
- Relay acts as a switch between the ESP32 (3.3V logic) and the pump (higher voltage load)
- No manual monitoring needed — fully autonomous

---

## 🛒 Hardware Components

| Component | Model | Purpose |
|-----------|-------|---------|
| Microcontroller | ESP32 (30-pin / 38-pin) | Central processing unit — reads sensors, controls actuators |
| Ultrasonic Sensor | HC-SR04 | Measures distance to detect person/object at door |
| Servo Motor | SG90 / MG90S | Physically opens and closes the door mechanism |
| Gas Sensor | MQ-2 | Detects LPG, methane, propane, smoke concentration |
| Buzzer | Active Buzzer Module | Audible gas alert |
| Water Level Sensor | Analog Water Level Sensor | Detects water tank fill status |
| Relay Module | 1-Channel 5V Relay | Switches water pump power on/off |
| Connecting Wires | Dupont Jumper Wires | Signal and power connections |
| Breadboard / PCB | Half-size breadboard | Prototyping connections |
| Power Supply | USB / 5V adapter | Powers ESP32 and modules |

---

## 📌 Pin Configuration

| Component | Pin | ESP32 GPIO |
|-----------|-----|------------|
| HC-SR04 TRIG | Output | GPIO 5 |
| HC-SR04 ECHO | Input | GPIO 18 |
| Servo Motor | Signal | GPIO 13 |
| MQ-2 Sensor | Analog Out | GPIO 34 (ADC) |
| Buzzer | Input | GPIO 12 |
| Water Level Sensor | Analog/Digital | GPIO 35 (ADC) |
| Relay Module | Control | GPIO 26 |

> **Note:** Pin assignments are defined as `#define` constants at the top of `SmartHome.ino`. Update these values if your wiring differs.

---

## 🧠 Working Logic

### Main Loop Flow

```
void loop() {

    ── DOOR SYSTEM ──────────────────────────────
    1. Trigger HC-SR04 pulse (10µs HIGH on TRIG)
    2. Measure echo duration (pulseIn on ECHO)
    3. Calculate distance in cm
    4. if distance ≤ 10 → servo.write(90)   // Open
       if distance > 15 → servo.write(0)    // Close

    ── GAS SYSTEM ───────────────────────────────
    5. Read MQ-2 analog value (analogRead)
    6. if gasValue > 800 → digitalWrite(BUZZER, HIGH)
       else              → digitalWrite(BUZZER, LOW)

    ── WATER SYSTEM ─────────────────────────────
    7. Read water level sensor (digitalRead or analogRead)
    8. if tankFull  → digitalWrite(RELAY, LOW)   // Pump OFF
       if tankEmpty → digitalWrite(RELAY, HIGH)  // Pump ON

}
```

All three systems run in every loop iteration — no `delay()` blocks that would pause sensor readings across subsystems.

---

## 📁 Code Structure

```
SmartHome.ino
│
├── Pin Definitions (#define)
│   ├── TRIG_PIN, ECHO_PIN       → Ultrasonic
│   ├── SERVO_PIN                → Servo Motor
│   ├── GAS_PIN                  → MQ-2 Sensor
│   ├── BUZZER_PIN               → Buzzer
│   ├── WATER_PIN                → Water Level Sensor
│   └── RELAY_PIN                → Relay Module
│
├── setup()
│   ├── Pin modes (INPUT / OUTPUT)
│   ├── Serial.begin(115200)     → Debug output
│   └── servo.attach(SERVO_PIN)  → Attach servo
│
└── loop()
    ├── readUltrasonic()         → Distance calculation
    ├── controlDoor()            → Servo write
    ├── readGasSensor()          → Analog read + threshold
    ├── controlBuzzer()          → Alert trigger
    ├── readWaterLevel()         → Level detection
    └── controlPump()            → Relay switch
```

---

## 📚 Libraries Used

| Library | Version | Purpose |
|---------|---------|---------|
| `ESP32Servo.h` | Latest | Servo motor control on ESP32 without PWM timer conflicts |
| `Arduino.h` | Built-in | Core Arduino functions (pinMode, digitalWrite, analogRead, pulseIn) |

**Install ESP32Servo via Arduino IDE:**
> Tools → Manage Libraries → Search `ESP32Servo` → Install

---

## 🔌 Circuit Diagram

The circuit diagram is included in the repository as `<img width="1169" height="885" alt="circuit-diagram" src="https://github.com/user-attachments/assets/51837acf-286b-42e7-ab88-b44075e6aeae" />
`.

**Quick wiring summary:**

```
HC-SR04:
  VCC  → 3.3V / 5V
  GND  → GND
  TRIG → GPIO 5
  ECHO → GPIO 18

Servo Motor:
  VCC (Red)    → 5V (external if needed)
  GND (Brown)  → GND
  Signal (Orange) → GPIO 13

MQ-2 Gas Sensor:
  VCC → 5V
  GND → GND
  AO  → GPIO 34

Buzzer:
  VCC → GPIO 12
  GND → GND

Water Level Sensor:
  VCC → 3.3V
  GND → GND
  S   → GPIO 35

Relay Module:
  VCC → 5V
  GND → GND
  IN  → GPIO 26
  COM/NO → Pump circuit
```

> ⚠️ The MQ-2 sensor requires a **warm-up period of ~60 seconds** after power-on for accurate readings.
> ⚠️ The Servo Motor may require an **external 5V supply** if the ESP32's 3.3V pin cannot supply enough current.

---

## ⚙️ Setup & Installation

### Prerequisites

- Arduino IDE 2.x installed ([Download](https://www.arduino.cc/en/software))
- ESP32 board package installed in Arduino IDE
- ESP32Servo library installed

### Step 1 — Install ESP32 Board Package

1. Open Arduino IDE → **File → Preferences**
2. Add this URL to "Additional Boards Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Go to **Tools → Board → Boards Manager**
4. Search `esp32` → Install **"esp32 by Espressif Systems"**

### Step 2 — Install ESP32Servo Library

1. Go to **Tools → Manage Libraries**
2. Search `ESP32Servo`
3. Install **"ESP32Servo by Kevin Harrington"**

### Step 3 — Clone the Repository

```bash
git clone https://github.com/soham-88/Smart-Home-Automation-ESP32.git
```

### Step 4 — Open the Sketch

1. Open Arduino IDE
2. Click **File → Open**
3. Navigate to the cloned folder → open `SmartHome.ino`

### Step 5 — Configure Your Pins

If your wiring differs from the default pin configuration, update the `#define` values at the top of `SmartHome.ino`:

```cpp
#define TRIG_PIN   5
#define ECHO_PIN   18
#define SERVO_PIN  13
#define GAS_PIN    34
#define BUZZER_PIN 12
#define WATER_PIN  35
#define RELAY_PIN  26
```

### Step 6 — Select Board & Port

1. **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
2. **Tools → Port → COMx** (select your ESP32's COM port)

### Step 7 — Upload

1. Hold the **BOOT button** on the ESP32 while clicking **Upload**
2. Release BOOT once "Connecting..." appears in the console
3. Wait for "Done uploading"

### Step 8 — Monitor

Open **Tools → Serial Monitor** at baud rate `115200` to see real-time sensor readings and system status.

---

## 🔮 Future Improvements

| Feature | Description |
|---------|-------------|
| 📱 Mobile App Control | Add Blynk or custom Android app to manually override door, buzzer, and pump from phone |
| ☁️ Cloud Monitoring | Integrate ThingSpeak or Arduino IoT Cloud to log sensor data and view dashboards remotely |
| 📲 Push Notifications | Send WhatsApp/SMS alerts via Twilio or CallMeBot when gas is detected |
| 🔐 Face / RFID Door Lock | Replace ultrasonic with RFID reader or face detection for secure entry |
| 🌡️ Temperature & Humidity | Add DHT22 sensor for climate monitoring inside the home |
| 💡 Smart Lighting | Add PIR sensor to control lights automatically when motion is detected |
| 🔋 Battery Backup | Add LiPo battery with charging circuit for power outage resilience |
| 📊 Local Web Dashboard | ESP32 hosts a local web server to show sensor status on any browser without internet |

---

## 👨‍💻 Developer

**Soham Gopal Pawar**
B.Sc. Computer Science (SYCS) — Bhavan's College, Mumbai

[![LinkedIn](https://img.shields.io/badge/LinkedIn-Connect-blue?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/soham-pawar-984b32319/)
[![GitHub](https://img.shields.io/badge/GitHub-Follow-black?style=for-the-badge&logo=github)](https://github.com/soham-88)

---

> ⭐ If you found this project useful or learned something from it, consider giving it a star on GitHub!
