#include <Arduino.h>

const int adcPin = 3; // ADC pin for voltage measurement
const int samples = 1000;
const int intervalMs = 7000; // 7s
const int sample_interval = 1; //1 ms

float initialTemp = 0;



float measureTemperatureAverage() {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += temperatureRead();
    delay(sample_interval);
  }
  return sum / samples;
}

void Main_Test(){
  float initial_temp = measureTemperatureAverage();
  Serial.print("Initial temperature: ");
  Serial.print(initial_temp, 3);
  Serial.println(" °C"); 
  delay(intervalMs);
  float last_temp = measureTemperatureAverage();
  Serial.print("Last temperature: ");
  Serial.print(last_temp, 3);
  Serial.println(" °C"); 
  Serial.print("Temperature difference: ");
  Serial.println(last_temp-initial_temp);
  if(last_temp-initial_temp<2){
    Serial.print("Test passed: True");
  }
  else{
    Serial.print("Test passed: False");
  }
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  for(int i = 0;i<10;i++){
    analogRead(adcPin);
    delay(10);
  }
  Serial.println("ESP32-C3 Serial Command Example. Type MAIN' to re-run script");

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

    if (command == "MAIN"){
      Main_Test();
    }else {
      Serial.println("Unknown command. Type MAIN");
    }
    while (Serial.available()) Serial.read();
  }
}
