#include <WiFi.h>
#include <esp_now.h>

#define LED1 2
#define LED2 15

typedef struct {

  bool btn1;

  bool btn2;

} ControlPacket;

ControlPacket packet;

// ===================== RECEIVE CALLBACK =====================
void onDataRecv(const uint8_t *mac,
                const uint8_t *incomingData,
                int len) {

  if (len != sizeof(packet)) {
    return;
  }

  memcpy(&packet, incomingData, sizeof(packet));

  digitalWrite(LED1, packet.btn1);

  digitalWrite(LED2, packet.btn2);

  Serial.println("==== RECEIVED ====");

  Serial.print("BTN1 : ");

  Serial.println(packet.btn1);

  Serial.print("BTN2 : ");

  Serial.println(packet.btn2);

  Serial.println();

}

void setup() {

  Serial.begin(115200);

  pinMode(LED1, OUTPUT);

  pinMode(LED2, OUTPUT);

  WiFi.mode(WIFI_STA);

  Serial.print("Receiver MAC : ");

  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {

    Serial.println("ESP NOW INIT FAILED");

    while (true);

  }

  esp_now_register_recv_cb(onDataRecv);

  Serial.println("RECEIVER READY");

}

void loop() {

}