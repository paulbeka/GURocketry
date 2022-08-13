// RSSI Test - Rocket
// Cameron Fraser | GUR Software | 5/8/22

// Libraries
#include <LoRa.h>             // LoRa transmission

// LoRa pins
#define RFM95_CS 10
#define RFM95_RST 22
#define RFM95_INT 23

// LoRa freq
#define LoRaFreq 433E6 // operating on 433MHz

// Global variable declarations
int messageNum = 0;

void setup() {
  // Serial init
  Serial.begin(9600);
  while (!Serial);
  Serial.println("GUR | RSSI Test - Rocket");


  // LoRa init
  Serial.println("Initialising LoRa...");
  LoRa.setPins(RFM95_CS, RFM95_RST, RFM95_INT);
  if (!LoRa.begin(LoRaFreq)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  LoRa.setTxPower(23); // increase power and therefore range by +20db (5-23 available)
  Serial.println("LoRa init successful.\n");

  
  Serial.println("Set up complete.\n");
}

void loop() {
  // Generate random data so we can test indoors
  int testData = random(100);


  // Send via LoRa
  LoRa.beginPacket();
  LoRa.print("#");
  LoRa.print(messageNum);
  LoRa.print(" ");
  LoRa.print(testData);
  LoRa.endPacket();
  messageNum++;
  Serial.print(testData); Serial.println(" successfully sent via LoRa.");
  delay(3000); // prevent spam
}
