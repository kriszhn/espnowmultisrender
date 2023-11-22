#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); 

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float temp;
  float hum;
} struct_message;

// Create a struct_message called myData
struct_message myData;
struct_message board1;
struct_message board2;

struct_message boardsStruct[3] = {board1, board2};


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) 
{
  //
   char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  //
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

  boardsStruct[myData.id-1].temp = myData.temp;
  boardsStruct[myData.id-1].hum = myData.hum;
   Serial.println("01");
  Serial.println(boardsStruct[0].temp);
  Serial.println(boardsStruct[0].hum);
 Serial.println("02");
  Serial.println(boardsStruct[1].temp);
 Serial.println(boardsStruct[1].hum);
  Serial.println();


  Serial.print("Bytes received: ");
  Serial.println(len);
  

  Serial.println();  delay(1000);

}
 
void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  Serial.println("ESPNow/Basic/Slave Example");
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
  Serial.println("Error initializing ESP-NOW");
  return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  float board1X = boardsStruct[0].temp;
  float board1Y = boardsStruct[0].hum;
  float board2X = boardsStruct[1].temp;
  float board2Y = boardsStruct[1].hum;
tft.fillScreen(TFT_BLACK);
 // tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 0, 2);
  tft.print("Room1_T: ");
  tft.println(board1X);
  tft.print("Room1_H: ");
  tft.println(board1Y);

  tft.print("Room2_T: ");
  tft.println(board2X);
  tft.print("Room1_H: ");
  tft.println(board2Y);

  delay(1000);

}
