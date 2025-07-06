#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* supabase_url = "*******************";
const char* apikey = "********************";

String ledStatusOfATtiny[11];

void getSeatData(){
  HTTPClient http;

  http.begin(supabase_url);
  http.addHeader("apikey", apikey);
  http.addHeader("Authorization", String("Bearer ") + apikey);

  int httpResponseCode = http.GET();

  if(httpResponseCode > 0){
    String payload = http.getString();
    Serial.println("Supabase data:");
    Serial.println(payload);
    parseSeats(payload);
  }
  else{
    Serial.println("Error code: " + String(httpResponseCode));
  }
}

void parseSeats(String payload){
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, payload);

  if(error){
    Serial.print("Json Parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  for(JsonObject seat: doc.as<JsonArray>()){
    int seat_no =  seat["seat_no."];
    const char* status = seat["status"];

    Serial.print("Seat ");
    Serial.print(seat_no);
    Serial.print(" => ");
    Serial.println(status);

  }
}

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password, 6);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  for(int i = 0; i < 12; i++){
    ledStatusOfATtiny[i] = "0000";
  }
  Serial.println(ledStatusOfATtiny[1]);

  getSeatData();
}

void loop(){
  delay(10000);
  getSeatData();  
}
