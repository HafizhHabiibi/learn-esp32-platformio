#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {

  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float temperaturetrue = dht.readTemperature(true);\

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.print("°C  Temperature (true): ");
  Serial.print(temperaturetrue);
  Serial.println("°F");
}