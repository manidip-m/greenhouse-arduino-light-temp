/*
  Greenhouse light and temperature monitor
  ----------------------------------------
  Board:   Arduino UNO
  Sensors: light sensor on A0, MCP9700A analog temperature sensor on A1
  Outputs: 4 indicator LEDs and a buzzer

  Every 30 s the sketch:
    1. reads temperature and raw light level,
    2. prints both to the serial monitor (9600 baud),
    3. switches the LEDs / buzzer according to the thresholds below.

  The serial output format must stay exactly as written, because the
  Python logger in python/ parses these two lines:
    Temperature (°C): <value>
    Light Intensity (Raw): <value>

  Pin map
    A0   light sensor input
    A1   temperature sensor input
    D8   blue LED    low light
    D9   yellow LED  high light
    D10  red LED     warm / high temperature
    D11  buzzer      hot (> 30 °C)
    D12  blue LED    low temperature
*/

// === Sensor input pins ===
const int LDR_PIN = A0;          // Analog pin connected to the light sensor
const int TEMP_SENSOR_PIN = A1;  // Analog pin connected to the MCP9700A-E/TO temperature sensor

// === Output pins (LEDs and buzzer) ===
const int LED_LOW_LIGHT = 8;    // Blue LED for low light (cloudy condition)
const int LED_HIGH_LIGHT = 9;   // Yellow LED for high light (sunny condition)
const int LED_HIGH_TEMP = 10;   // Red LED for high temperature warning
const int LED_LOW_TEMP = 12;    // Blue LED for low temperature indication
const int BUZZER = 11;          // Buzzer for high temperature alert

// === Light thresholds (raw ADC units, 0-1023) ===
const int LIGHT_LOW_THRESHOLD = 800;   // Below this = cloudy (low light)
const int LIGHT_HIGH_THRESHOLD = 980;  // Above this = sunny (high light)

// === Temperature thresholds (°C) ===
const float TEMP_THRESHOLD_LOW = 20.0;     // Below this = cold -> blue LED ON
const float TEMP_THRESHOLD_LED = 25.0;     // Above this = warm -> red LED ON
const float TEMP_THRESHOLD_BUZZER = 30.0;  // Above this = hot  -> buzzer ON

// === MCP9700A constants ===
const float TEMP_SENSOR_V_OFFSET = 0.5;  // 500 mV = 0 °C
const float TEMP_SENSOR_SCALE = 0.01;    // 10 mV per 1 °C

void setup() {
  // Set output pins for LEDs and buzzer
  pinMode(LED_LOW_LIGHT, OUTPUT);
  pinMode(LED_HIGH_LIGHT, OUTPUT);
  pinMode(LED_HIGH_TEMP, OUTPUT);
  pinMode(LED_LOW_TEMP, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Start serial communication for monitoring and logging
  Serial.begin(9600);
}

void loop() {
  // === Read temperature sensor ===
  int tempRaw = analogRead(TEMP_SENSOR_PIN);                // Raw ADC value (0-1023)
  float tempVoltage = tempRaw * (5.0 / 1024.0);             // Convert to volts (0-5 V)
  float temperatureC = (tempVoltage - TEMP_SENSOR_V_OFFSET) / TEMP_SENSOR_SCALE;  // Volts -> °C

  delay(100);  // Short pause so the two analog readings do not interfere

  // === Read light sensor ===
  int lightRaw = analogRead(LDR_PIN);  // Raw ADC value (0-1023)

  // === Print readings to the serial monitor ===
  Serial.print("Temperature (°C): ");
  Serial.println(temperatureC);
  Serial.print("Light Intensity (Raw): ");
  Serial.println(lightRaw);

  // === Light intensity alerts ===
  if (lightRaw < LIGHT_LOW_THRESHOLD) {
    // Cloudy condition -> blue LED on
    digitalWrite(LED_LOW_LIGHT, HIGH);
    digitalWrite(LED_HIGH_LIGHT, LOW);
  } else if (lightRaw > LIGHT_HIGH_THRESHOLD) {
    // Sunny condition -> yellow LED on
    digitalWrite(LED_HIGH_LIGHT, HIGH);
    digitalWrite(LED_LOW_LIGHT, LOW);
  } else {
    // In-between values -> both LEDs off
    digitalWrite(LED_LOW_LIGHT, LOW);
    digitalWrite(LED_HIGH_LIGHT, LOW);
  }

  // === Temperature alerts ===
  if (temperatureC < TEMP_THRESHOLD_LOW) {
    digitalWrite(LED_LOW_TEMP, HIGH);   // Too cold -> blue LED on
  } else {
    digitalWrite(LED_LOW_TEMP, LOW);    // Normal/warm -> blue LED off
  }

  if (temperatureC > TEMP_THRESHOLD_LED) {
    digitalWrite(LED_HIGH_TEMP, HIGH);  // Too warm -> red LED on
  } else {
    digitalWrite(LED_HIGH_TEMP, LOW);   // Normal/cool -> red LED off
  }

  if (temperatureC > TEMP_THRESHOLD_BUZZER) {
    tone(BUZZER, 500);                  // Too hot -> buzzer at 500 Hz
  } else {
    noTone(BUZZER);                     // Temperature safe -> buzzer off
  }

  delay(30000);  // Wait 30 seconds before the next reading
}
