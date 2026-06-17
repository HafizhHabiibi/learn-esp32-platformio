#define BLYNK_TEMPLATE_ID "TMPL6WUhLgdsv"
#define BLYNK_TEMPLATE_NAME "Monitoring Ruangan IoT"
#define BLYNK_AUTH_TOKEN "9NpEeUxqirY9ZzdPh5gwduqngJT7uZ5v"

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

const int MQ135Pin = 34;
const int LEDPin = 25;
const int DHTPin = 27;
const int RelayPin = 26;

// Batas Bawah untuk ADC
const int MIN_THRESHOLD = 100;

#define DHTTYPE DHT22
DHT dht(DHTPin, DHTTYPE);

BlynkTimer timer;

String getAirQuality(float ppm) {
  if (ppm < 600) return "Clean";
  else if (ppm < 1000) return "Normal";
  else if (ppm < 1500) return "Polluted";
  else return "Danger";
}

// Konversi ADC ke PPM Dummy
float calculatePPM(int rawValue) {
  float ppm = map(rawValue, MIN_THRESHOLD, 3000, 400, 2000);
  if (ppm < 400) ppm = 400;
  if (ppm > 2000) ppm = 2000;

  return ppm;
}

void sendDataBlynk() {
  int sum = 0;
  for(int i = 0; i < 10; i++) {
    sum += analogRead(MQ135Pin);
    delay(10);
  }
  int rawValue = sum / 10;

  if (rawValue < MIN_THRESHOLD) {
    rawValue = MIN_THRESHOLD;
  }

  // pembacaan sensor MQ135 dan konversi ke PPM
  float ppm = calculatePPM(rawValue);

  // Pembacaan sensor DHT22
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  String status = getAirQuality(ppm);

  // Logic Led
  if(status == "Polluted" || status == "Danger") {
    digitalWrite(LEDPin, HIGH);                       
  } else {
    digitalWrite(LEDPin, LOW);
  }

  // Logic Relay
  if(temperature > 30) {
    digitalWrite(RelayPin, HIGH);                       
  } else {
    digitalWrite(RelayPin, LOW);
  }

  // Kirim data ke Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, ppm);
  Blynk.virtualWrite(V3, status);
  Blynk.virtualWrite(V4, digitalRead(LEDPin));
  Blynk.virtualWrite(V5, digitalRead(RelayPin));

  // Serial Monitor
  Serial.print("ADC: ");
  Serial.print(rawValue);

  Serial.print(" | PPM: ");
  Serial.print(ppm);

  Serial.print(" | Status: ");
  Serial.print(status);

  Serial.print(" | Humidity: ");
  Serial.print(humidity);

  Serial.print("% | Temp: ");
  Serial.println(temperature);
}

void setup() {
  Serial.begin(115200);

  dht.begin();
  analogSetAttenuation(ADC_11db);
  
  pinMode(LEDPin, OUTPUT);
  pinMode(RelayPin, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendDataBlynk);
}

void loop() {
  Blynk.run();
  timer.run();
}