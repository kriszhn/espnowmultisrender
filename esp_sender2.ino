#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>
#define DHTPIN 4 
#define DHTTYPE    DHT11 

DHT dht(DHTPIN, DHTTYPE);
uint8_t broadcastAddress[] = {0x78, 0x21, 0x84, 0x88, 0xB2, 0x04};
//A8:42:E3:47:A4:14


typedef struct {
  int id;
  float temp;
  float hum;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  //memset(&myData, 0, sizeof(myData));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.print("Temperature: "); 
  Serial.println(t);
  Serial.print("Humidity: "); 
  Serial.println(h);
  myData.id = 2;
  myData.temp = t;
  myData.hum = h;

  Serial.print("Sending: "); Serial.println(myData.temp);
  Serial.print("Sendingx: "); Serial.println(t);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
