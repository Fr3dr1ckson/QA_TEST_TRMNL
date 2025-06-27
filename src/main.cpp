#include <Arduino.h>

const int adcPin = 3; // ADC pin for voltage measurement
const int samples = 1000;
const int intervalMs = 7000; // 50 ms
const int sample_interval = 1; //1 ms

float initialTemp = 0;
float initialVolt = 0;

void voltageStatus(float voltage) {
  if (voltage > 4.2) {
    Serial.println("BATTERY EITHER FULL CHARGED OR NOT CONNECTED, PLEASE CHECK IF DEVICE MEETS ALL THE SPECIFIED CONDITIONS");
  }
}

float voltage_measure() {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    int adcValue = analogReadMilliVolts(adcPin);
    float voltage = ((float)adcValue);
    sum += voltage;
    delay(sample_interval);
  }
  return sum/samples;
}

float measureTemperatureAverage() {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += temperatureRead();
    delay(sample_interval);
  }
  return sum / samples;
}

void Main_Test(){
  float initial_voltage = voltage_measure();
  float initial_temp = measureTemperatureAverage();
  Serial.print("Initial temperature: ");
  Serial.print(initial_temp, 3);
  Serial.println(" °C"); 
  Serial.print("Initial voltage: ");
  Serial.print(initial_voltage, 3);
  Serial.println(" V"); 
  delay(intervalMs);
  float last_voltage = voltage_measure();
  float last_temp = measureTemperatureAverage();
  Serial.print("Last temperature: ");
  Serial.print(last_temp, 3);
  Serial.println(" °C"); 
  Serial.print("Last voltage: ");
  Serial.print(last_voltage, 3);
  Serial.println(" V"); 
  Serial.print("Voltage difference: ");
  Serial.println(last_voltage - initial_voltage);
  Serial.print("Temperature difference: ");
  Serial.println(last_temp-initial_temp);
}
float Voltage_Test() {
  float sum = 0;
  Serial.println("Measuring voltage (100 samples in 5 seconds)...");
  for (int i = 0; i < samples; i++) {
    float voltage = voltage_measure();
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

 
   {
    if (average > initialVolt + 0.01) {
      Serial.println("Battery voltage has increased by");
      Serial.print((average-initialVolt-0.01));
      Serial.print(" since startup");
    } else if (average < initialVolt - 0.01) {
      Serial.println("Battery voltage has decreased by");
      Serial.print(initialVolt-average-0.01);
      Serial.print(" since startup");
    } else {
      Serial.println("Battery voltage unchanged since startup.");
    }
  }
  return average;
}





void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-C3 Serial Command Example. Type 'TEMP' or 'VOLT' to start.");

  // Take initial measurements
  Main_Test();

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
    Serial.print("Temperature has increased by ");
    Serial.print((average-initialTemp-0.2));
    Serial.print(" since startup");
  } else if (average < initialTemp - 0.2) {
    Serial.print("Temperature has decreased since startup by ");
    Serial.print((initialTemp-average-0.2));
    Serial.print(" since startup");
  } else {
    Serial.println("Temperature unchanged since startup.");
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
      Voltage_Test();
    } else if (command == "MAIN"){
      Main_Test();
    }else {
      Serial.println("Unknown command. Type 'TEMP' or 'VOLT'.");
    }
    while (Serial.available()) Serial.read();
  }
}
