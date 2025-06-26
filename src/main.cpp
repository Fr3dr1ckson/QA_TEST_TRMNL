#include <Arduino.h>

const int adcPin = 3; // ADC pin for voltage measurement
const int samples = 20;
const int durationMs = 10000; // 10 seconds
const int intervalMs = durationMs / samples; // 500 ms

float initialTemp = 0;
float initialVolt = 0;

float measureTemperatureAverage() {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += temperatureRead();
    delay(intervalMs);
  }
  return sum / samples;
}

float measureVoltageAverage() {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    int adcValue = analogReadMilliVolts(adcPin);
    float voltage = (((float)adcValue) * 2) / 1000.0;
    sum += voltage;
    delay(intervalMs);
  }
  return sum / samples;
}

void voltageStatus(float voltage) {
  if (voltage > 4.5) {
    Serial.println("NO BATTERY DETECTED");
  } else if (voltage >= 4.15 && voltage <= 4.25) {
    Serial.println("BATTERY FULLY CHARGED");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-C3 Serial Command Example. Type 'TEMP' or 'VOLT' to start.");

  // Take initial measurements
  Serial.println("Taking initial temperature measurement...");
  initialTemp = measureTemperatureAverage();
  Serial.print("Initial temperature average: ");
  Serial.print(initialTemp);
  Serial.println(" °C");

  Serial.println("Taking initial voltage measurement...");
  initialVolt = measureVoltageAverage();
  Serial.print("Initial voltage average: ");
  Serial.print(initialVolt, 3);
  Serial.println(" V");
  voltageStatus(initialVolt);
}

void measureTemp() {
  float sum = 0;
  Serial.println("Measuring temperature (20 samples in 10 seconds)...");
  for (int i = 0; i < samples; i++) {
    float tempC = temperatureRead();
    sum += tempC;
    Serial.print("Sample ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(tempC);
    Serial.println(" °C");
    delay(intervalMs);
  }
  float average = sum / samples;
  Serial.print("Average temperature: ");
  Serial.print(average);
  Serial.println(" °C");

  // Compare to initial
  if (average > initialTemp + 0.2) {
    Serial.println("Temperature has increased since startup.");
  } else if (average < initialTemp - 0.2) {
    Serial.println("Temperature has decreased since startup.");
  } else {
    Serial.println("Temperature unchanged since startup.");
  }
}

void measureVolt() {
  float sum = 0;
  Serial.println("Measuring voltage (20 samples in 10 seconds)...");
  for (int i = 0; i < samples; i++) {
    int adcValue = analogReadMilliVolts(adcPin);
    float voltage = (((float)adcValue) * 2) / 1000.0;
    sum += voltage;
    Serial.print("Sample ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(voltage, 3);
    Serial.println(" V");
    delay(intervalMs);
  }
  float average = sum / samples;
  Serial.print("Average voltage: ");
  Serial.print(average, 3);
  Serial.println(" V");
  voltageStatus(average);

  // Only check/compare if voltage in expected range
  if (!(average > 4.5 || (average >= 4.15 && average <= 4.25))) {
    if (average > initialVolt + 0.01) {
      Serial.println("Battery voltage has increased since startup.");
    } else if (average < initialVolt - 0.01) {
      Serial.println("Battery voltage has decreased since startup.");
    } else {
      Serial.println("Battery voltage unchanged since startup.");
    }
  }
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();

    if (command == "TEMP") {
      measureTemp();
    } else if (command == "VOLT") {
      measureVolt();
    } else {
      Serial.println("Unknown command. Type 'TEMP' or 'VOLT'.");
    }
    while (Serial.available()) Serial.read();
  }
}
