#include <math.h> // Include math library for future calculations if needed

// === Sensor Input Pins ===
const int LDR_PIN = A0; // Analog pin connected to LDR (light sensor)
const int TEMP_SENSOR_PIN = A1; // Analog pin connected to MCP9700A-E/TO temperature sensor

// === Output Pins (LEDs and Buzzer) ===
const int LED_LOW_LIGHT = 8; // Blue LED for low light (cloudy condition)
const int LED_HIGH_LIGHT = 9; // Yellow LED for high light (sunny condition)
const int LED_HIGH_TEMP = 10; // Red LED for high temperature warning
const int LED_LOW_TEMP = 12; // Green LED for low temperature indication
const int BUZZER = 11; // Buzzer for high temperature alert

// === Light Thresholds ===
const int LIGHT_LOW_THRESHOLD = 800; // Below this = cloudy (low light)
const int LIGHT_HIGH_THRESHOLD = 980; // Above this = sunny (high light)

// === Temperature Thresholds (in °C) ===
const float TEMP_THRESHOLD_LOW = 20.0; // Below this = cold → green LED ON
const float TEMP_THRESHOLD_LED = 25.0; // Above this = warm → red LED ON
const float TEMP_THRESHOLD_BUZZER = 30.0; // Above this = hot → buzzer ON

// === MCP9700A Constants ===
const float TEMP_SENSOR_V_OFFSET = 0.5; // 500mV = 0°C
const float TEMP_SENSOR_SCALE = 0.01; // 10mV per 1°C

void setup() {
  // Set output pins for LEDs and buzzer
  pinMode(LED_LOW_LIGHT, OUTPUT);
  pinMode(LED_HIGH_LIGHT, OUTPUT);
  pinMode(LED_HIGH_TEMP, OUTPUT);
  pinMode(LED_LOW_TEMP, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Start serial communication for monitoring
  Serial.begin(9600);
}

void loop() {
  // === Read Temperature Sensor ===
  int tempRaw = analogRead(TEMP_SENSOR_PIN); // Read analog value from temperature sensor
  float tempVoltage = tempRaw * (5.0 / 1024.0); // Convert analog value to voltage (0–5V range)
  float temperatureC = (tempVoltage - TEMP_SENSOR_V_OFFSET) / TEMP_SENSOR_SCALE; // Convert voltage to °C

  delay(100); // Small delay to allow sensor to stabilize

  // === Read Light Sensor (LDR) ===
  int lightRaw = analogRead(LDR_PIN); // Read raw analog value from light sensor (0–1023)

  // === Print Sensor Readings to Serial Monitor ===
  Serial.print("Temperature (°C): ");
  Serial.println(temperatureC);
  Serial.print("Light Intensity (Raw): ");
  Serial.println(lightRaw);

  // === Light Intensity Alerts ===
  if (lightRaw < LIGHT_LOW_THRESHOLD) {
    // Cloudy condition → turn on blue LED
    digitalWrite(LED_LOW_LIGHT, HIGH);
    digitalWrite(LED_HIGH_LIGHT, LOW);
  } else if (lightRaw > LIGHT_HIGH_THRESHOLD) {
    // Sunny condition → turn on yellow LED
    digitalWrite(LED_HIGH_LIGHT, HIGH);
    digitalWrite(LED_LOW_LIGHT, LOW);
  } else {
    // In-between values → turn off both LEDs
    digitalWrite(LED_LOW_LIGHT, LOW);
    digitalWrite(LED_HIGH_LIGHT, LOW);
  }

  // === Temperature Alerts ===
  if (temperatureC < TEMP_THRESHOLD_LOW) {
    // Too cold → turn on green LED
    digitalWrite(LED_LOW_TEMP, HIGH);
  } else {
    // Normal/warm → turn off green LED
    digitalWrite(LED_LOW_TEMP, LOW);
  }

  if (temperatureC > TEMP_THRESHOLD_LED) {
    // Too warm → turn on red LED
    digitalWrite(LED_HIGH_TEMP, HIGH);
  } else {
    // Normal/cool → turn off red LED
    digitalWrite(LED_HIGH_TEMP, LOW);
  }

  if (temperatureC > TEMP_THRESHOLD_BUZZER) {
    // Too hot → activate buzzer at 500 Hz
    tone(BUZZER, 500);
  } else {
    // Temperature safe → turn off buzzer
    noTone(BUZZER);
  }

  delay(30000); // Wait 30 seconds before next reading
}
