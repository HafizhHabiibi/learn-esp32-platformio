#include <Arduino.h>

const int PH_PIN = 34;
const float ADC_CAL_FACTOR = 2.500 / 2.294;

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  Serial.println("Kalibrasi PH-4502C");
}

void loop() {
  long total = 0;
  const int samples = 100;

  for (int i = 0; i < samples; i++) {
    total += analogRead(PH_PIN);
    delay(5);
  }

  float adcValue = total / (float)samples;

  float voltage = adcValue * (3.3 / 4095.0);
  voltage *= ADC_CAL_FACTOR;

  Serial.print("ADC: ");
  Serial.print(adcValue, 0);

  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.println(" V");

  delay(1000);
}