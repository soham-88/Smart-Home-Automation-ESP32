# 🏠 Smart Home Automation using ESP32

## 📖 Project Overview
This project is a Smart Home Automation system developed using ESP32. It integrates multiple automation features including automatic door control, gas leakage detection, and automatic water tank management.

The system improves safety, convenience, and efficient water usage.

---

## ⚙️ Features

### 🚗 1. Automatic Door System
- Uses Ultrasonic Sensor to detect object/person
- If distance ≤ 10cm → Door opens
- If distance > 15cm → Door closes
- Controlled using Servo Motor

### 🔥 2. Gas Leakage Detection
- Uses MQ-2 Gas Sensor
- If gas level exceeds threshold (800):
  - Buzzer turns ON
- If safe:
  - Buzzer turns OFF

### 💧 3. Automatic Water Tank Control
- Uses Water Level Sensor
- If tank FULL:
  - Pump turns OFF
- If tank NOT full:
  - Pump turns ON
- Controlled using Relay Module

---

## 🛠 Hardware Components
- ESP32
- Ultrasonic Sensor (HC-SR04)
- Servo Motor
- MQ-2 Gas Sensor
- Water Level Sensor
- Relay Module
- Buzzer
- Connecting Wires

---

## 🧠 Working Logic
The ESP32 continuously monitors:
- Distance from Ultrasonic sensor
- Gas levels from MQ-2 sensor
- Water level status

Based on sensor readings, it automatically controls:
- Servo Motor (Door)
- Buzzer (Gas Alert)
- Water Pump (Relay)

---

## 💻 Programming
- Language: Embedded C (Arduino IDE)
- Library Used: ESP32Servo.h

---

## 📷 Project Images
(Add your circuit diagram and project images here)

---

## 📚 Learning Outcomes
- ESP32 Programming
- Sensor Integration
- Embedded Systems Logic
- Real-time Monitoring
- Hardware & Software Integration

---

## 🚀 Future Improvements
- Mobile App Control
- IoT Cloud Monitoring
- SMS/Notification Alerts
