// RSSI Test - Ground Station
// Cameron Fraser | GUR Software | 5/8/22

// Libraries
#include <SPI.h>
#include <LoRa.h>

// LoRa pins
#define RFM95_CS 10
#define RFM95_RST 22
#define RFM95_INT 23

// LoRa Freq
#define LoRaFreq 433E6 // operating on 433MHz

void setup() {
  // Serial init
  Serial.begin(9600);
  while (!Serial);
  Serial.println("GUR | RSSI Test - Ground Station");

  // LoRa init
  Serial.println("Initialising LoRa...");
  LoRa.setPins(RFM95_CS, RFM95_RST, RFM95_INT);
  if (!LoRa.begin(LoRaFreq)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  LoRa.setTxPower(20); // increase power and therefore range by +20db (5-23 available)
  Serial.println("LoRa init successful.\n");
  Serial.println("Set up complete.\n");

  Serial.println("Awaiting GPS Data...");
}

void loop() {
  int charCounter = 0;
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
      Serial.print("Data recieved: ");

      while (LoRa.available()){
        char incoming = (char)LoRa.read(); 
        
        if (charCounter == 0 && incoming != '#') {
          // if message doesn't begin with '#' we don't want it
          break;
        }

        Serial.print(incoming);
        charCounter++;
      }

      Serial.print(" | RSSI: ");
      Serial.print(LoRa.packetRssi());
      Serial.print("\n");
   }
}
