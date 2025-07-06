#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define AT_UART_TX 17
#define AT_UART_RX -1

const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";

const char* SUPABASE_URL = "*******************";
const char* APIKEY = "********************";

String ledStatusOfATtiny[11];
HTTPClient http;

void getSeatData(){
  http.begin(SUPABASE_URL);
  http.addHeader("apikey", APIKEY);
  http.addHeader("Authorization", String("Bearer ") + APIKEY);

  int httpResponseCode = http.GET();

  if(httpResponseCode > 0){
    String payload = http.getString();
    Serial.println("Payload Size: " + String(payload.length()));
    // Serial.println("Supabase data:");
    // Serial.println(payload);
    parseSeats(payload);
  }
  else{
    Serial.println("Error code: " + String(httpResponseCode));
  }
}

void parseSeats(String payload){
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, payload);

  for(int i = 0; i < 11; i++){
    ledStatusOfATtiny[i] = "0000";
  }
  
  if(error){
    Serial.print("Json Parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  for(JsonObject seat: doc.as<JsonArray>()){
    int seatNum =  seat["seat_no."];
    String statusStr = String(seat["status"]);
    statusStr.trim();

    int attinyNum = (seatNum - 1) / 4;
    int ledNum = (seatNum - 1) % 4;

    // Serial.print("Seat ");
    // Serial.print(seatNum);
    // Serial.print(" => ");
    // Serial.print(statusStr);
    // Serial.print(" => ATtiny ");
    // Serial.print(attinyNum + 1);
    // Serial.print(", LED ");
    // Serial.println(ledNum + 1);

    if (statusStr == "booked") {
      ledStatusOfATtiny[attinyNum].setCharAt(ledNum, '1');
    }
  }
}

void setup(){
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, AT_UART_RX, AT_UART_TX);
  
  WiFi.begin(SSID, PASSWORD, 6);
  // Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    // Serial.print(".");
  }
  // Serial.println("Connected!");
  http.setReuse(true);
  for(int i = 0; i < 12; i++){
    ledStatusOfATtiny[i] = "0000";
  }
}

void loop(){
  
  unsigned long startMillis = millis();
  getSeatData();
  unsigned long endMillis = millis();
  Serial.print("getSeatData() took: ");
  Serial.print((endMillis - startMillis)/1000);
  Serial.println(" secs");

  // Serial.println("ATtiny LED status overview:");
  // for (int i = 0; i < 12; i++) {
  //   Serial.print("ATtiny ");
  //   Serial.print(i + 1);
  //   Serial.print(": ");
  //   Serial.println(ledStatusOfATtiny[i]);
  // }

  unsigned long currentMillis = millis();
  Serial.print("Completed at: ");
  Serial.print(currentMillis / 1000);
  Serial.println(" seconds since start");
  Serial.println("=====================================");
  delay(10000);
 
}
