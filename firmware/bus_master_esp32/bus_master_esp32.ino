#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <esp_now.h>
#include <ArduinoJson.h>
#include <esp_wifi.h>


// ------------------- WiFi & Firebase -------------------
const char* ssid = "ssid";
const char* password = "password";
const char* FIREBASE_TOKEN = "firebase_api_token";
const char* FIREBASE_URL = "https://firestore.googleapis.com/v1/projects/utpsmartride-db/databases/(default)/documents/buses/Bus%20A/seats?access_token=";

// ------------------- ESP-NOW Nodes -------------------
uint8_t NODE_MACS[][6] = {
  {0x28,0x05,0xA5,0x6E,0x63,0x4C}, // Node 1
  {0x28,0x05,0xA5,0x70,0x8F,0x14}  // Node 2
};
const int NUM_NODES = sizeof(NODE_MACS)/6;

// ------------------- Functions -------------------
void sendBitmap(uint8_t* mac, uint8_t bitmap){
  esp_err_t result = esp_now_send(mac, &bitmap, sizeof(bitmap));
  if(result == ESP_OK){
    Serial.printf("Sent bitmap %02X to node\n", bitmap);
  } else {
    Serial.printf("Error sending bitmap %02X\n", bitmap);
  }
}

void setup() {
  Serial.begin(115200);

  // Connect WiFi
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  // Lock ESP-NOW to the same channel
  uint8_t wifi_channel = WiFi.channel();
  esp_wifi_set_channel(wifi_channel, WIFI_SECOND_CHAN_NONE);
  Serial.printf("ESP-NOW set to channel %d\n", wifi_channel);

  // Init ESP-NOW
  if(esp_now_init() != ESP_OK){
    Serial.println("ESP-NOW init failed"); while(true);
  }

  for(int i=0;i<NUM_NODES;i++){
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, NODE_MACS[i], 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if(esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
    }
  }
}

void loop() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;
  https.begin(client, String(FIREBASE_URL) + FIREBASE_TOKEN);
  int httpCode = https.GET();

  if(httpCode == 200){
    String payload = https.getString();

    // Parse JSON
    const size_t capacity = 2048;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, payload);

    JsonArray docs = doc["documents"].as<JsonArray>();

    // Generate bitmaps per node
    for(int node=0; node<NUM_NODES; node++){
      uint8_t bitmap = 0;
      int start_seat = node*4 + 1;
      int end_seat = start_seat + 3;

      for(JsonObject seat : docs){
        String name = seat["name"].as<String>();
        String seat_number_str = name.substring(name.lastIndexOf("Seat ") + 5);
        int seat_number = seat_number_str.toInt();
        String status = seat["fields"]["status"]["stringValue"].as<String>();

        if(seat_number >= start_seat && seat_number <= end_seat && status == "taken"){
          bitmap |= (1 << ((seat_number-1)%4));
        }
      }

      sendBitmap(NODE_MACS[node], bitmap);
    }

  } else {
    Serial.printf("HTTP GET failed: %d\n", httpCode);
  }
  https.end();

  delay(2000); // poll every 2 seconds
}
