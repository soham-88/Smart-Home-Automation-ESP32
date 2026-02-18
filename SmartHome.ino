#include <ESP32Servo.h>

/* -------- PIN DEFINITIONS -------- */
// GAS
#define MQ_PIN 34
#define BUZZER_PIN 25
#define GAS_THRESHOLD 800

// WATER
#define WATER_SENSOR 27
#define RELAY_PIN 26

// DOOR
#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 13

/* -------- OBJECTS & VARIABLES -------- */
Servo doorServo;
bool doorOpen = false;

void setup() {
  Serial.begin(115200);

  /* ---- DOOR SETUP ---- */
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // Door closed

  /* ---- GAS SETUP ---- */
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  /* ---- WATER SETUP ---- */
  pinMode(WATER_SENSOR, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Pump ON initially

  // MQ-2 Warm-up
  Serial.println("MQ-2 warming up...");
  delay(30000);
  Serial.println("System Ready");
}

void loop() {

  /* ---------- DOOR SYSTEM ---------- */
  long duration;
  int distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 25000);
  distance = duration * 0.034 / 2;

  if (distance > 0 && distance <= 10 && !doorOpen) {
    doorServo.write(90);
    doorOpen = true;
    Serial.println("🚗 Door Opened");
    delay(2000);
  }

  if (distance > 15 && doorOpen) {
    doorServo.write(0);
    doorOpen = false;
    Serial.println("🚧 Door Closed");
  }

  /* ---------- GAS SYSTEM (SILENT) ---------- */
  int gas = analogRead(MQ_PIN);

  if (gas > GAS_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Buzzer OFF
  }

  /* ---------- WATER SYSTEM ---------- */
  int waterStatus = digitalRead(WATER_SENSOR);

  if (waterStatus == HIGH) {
    digitalWrite(RELAY_PIN, HIGH); // Pump OFF
    Serial.println("💧 Tank FULL → Pump OFF");
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Pump ON
    Serial.println("💧 Tank Filling → Pump ON");
  }

  delay(1000);
}