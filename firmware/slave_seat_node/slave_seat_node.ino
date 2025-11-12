#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

// ------------------- Node Config -------------------
// Buzzer
#define BUZZER_PIN 4

// Each seat bi-color LED (common GND)
#define LED1_R 12
#define LED1_G 13
#define LED2_R 14
#define LED2_G 15
#define LED3_R 16
#define LED3_G 17
#define LED4_R 18
#define LED4_G 19

// Ultrasonic sensors: trig / echo
#define US1_TRIG 21
#define US1_ECHO 25
#define US2_TRIG 23
#define US2_ECHO 22
#define US3_TRIG 26
#define US3_ECHO 27
#define US4_TRIG 32
#define US4_ECHO 33

// Threshold distance (cm) — below this means someone is sitting
#define OCCUPIED_DIST 30

uint8_t bitmap = 0;

// ------------------- Helper Functions -------------------
float getDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);  // timeout 30ms
  float distance = duration * 0.0343 / 2;         // convert to cm
  return distance > 0 ? distance : 999;           // return large value if timeout
}

void updateSeatLEDs(uint8_t bits) {
  digitalWrite(LED1_R, bits & 0x01 ? HIGH : LOW);
  digitalWrite(LED1_G, bits & 0x01 ? LOW : HIGH);
  digitalWrite(LED2_R, bits & 0x02 ? HIGH : LOW);
  digitalWrite(LED2_G, bits & 0x02 ? LOW : HIGH);
  digitalWrite(LED3_R, bits & 0x04 ? HIGH : LOW);
  digitalWrite(LED3_G, bits & 0x04 ? LOW : HIGH);
  digitalWrite(LED4_R, bits & 0x08 ? HIGH : LOW);
  digitalWrite(LED4_G, bits & 0x08 ? LOW : HIGH);
}

void onDataReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != 1) return;  // Expect 1 byte (4 seats)
  bitmap = data[0];

  Serial.printf("Received bitmap: %02X from %02X:%02X:%02X:%02X:%02X:%02X\n",
                bitmap,
                info->src_addr[0], info->src_addr[1], info->src_addr[2],
                info->src_addr[3], info->src_addr[4], info->src_addr[5]);

  updateSeatLEDs(bitmap);
}

void checkSeatOccupancy() {
  // Read ultrasonic distances
  float d1 = getDistanceCM(US1_TRIG, US1_ECHO);
  float d2 = getDistanceCM(US2_TRIG, US2_ECHO);
  float d3 = getDistanceCM(US3_TRIG, US3_ECHO);
  float d4 = getDistanceCM(US4_TRIG, US4_ECHO);

  bool alert = false;

  // Check if a booked seat is physically occupied
  if ((bitmap & 0x01) && d1 < OCCUPIED_DIST) alert = true;
  if ((bitmap & 0x02) && d2 < OCCUPIED_DIST) alert = true;
  if ((bitmap & 0x04) && d3 < OCCUPIED_DIST) alert = true;
  if ((bitmap & 0x08) && d4 < OCCUPIED_DIST) alert = true;

  if (alert) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("⚠️  Someone sitting on a booked seat!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.printf("Distances: %.1f, %.1f, %.1f, %.1f cm\n", d1, d2, d3, d4);
}

// ------------------- Setup -------------------
void setup() {
  Serial.begin(115200);

  // LED setup
  pinMode(LED1_R, OUTPUT); pinMode(LED1_G, OUTPUT);
  pinMode(LED2_R, OUTPUT); pinMode(LED2_G, OUTPUT);
  pinMode(LED3_R, OUTPUT); pinMode(LED3_G, OUTPUT);
  pinMode(LED4_R, OUTPUT); pinMode(LED4_G, OUTPUT);

  // Ultrasonic setup
  pinMode(US1_TRIG, OUTPUT); pinMode(US1_ECHO, INPUT);
  pinMode(US2_TRIG, OUTPUT); pinMode(US2_ECHO, INPUT);
  pinMode(US3_TRIG, OUTPUT); pinMode(US3_ECHO, INPUT);
  pinMode(US4_TRIG, OUTPUT); pinMode(US4_ECHO, INPUT);

  // Buzzer setup
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // WiFi + ESP-NOW
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE); // Match master's channel

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    while (true);
  }

  esp_now_register_recv_cb(onDataReceive);

  updateSeatLEDs(0x00);  // all green initially
}

// ------------------- Loop -------------------
void loop() {
  checkSeatOccupancy();
  delay(1000); // check every 1s
}
