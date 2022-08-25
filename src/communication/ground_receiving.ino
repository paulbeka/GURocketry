/* GU Rocketry - Saltire 2 | Ground Station | 25/08/22 */
#include <LoRa.h>
#include <SD.h>
#include <SPI.h>

#define LoRaFreq 433E6
#define RFM95_CS 10
#define RFM95_RST 22
#define RFM95_INT 23

char filename[9] = "data.txt";

void writeToFile(String message) {
  File sdFile = SD.open(filename, FILE_WRITE);
  sdFile.println(message);
  sdFile.close();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("GU Rocketry - Saltire 2 | Ground Station\n");

  Serial.println("Initialising LoRa...");
  LoRa.setPins(RFM95_CS, RFM95_RST, RFM95_INT);
  if (!LoRa.begin(LoRaFreq)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init successful.\n");
  LoRa.setTxPower(23);

  while (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Error initialising SD card!");
    delay(1000);
  }
  Serial.println("Initialised SD card.");
}

void loop() {
  int packetSize = LoRa.parsePacket();

  String message = "";

  if (packetSize) {
      while (LoRa.available()) {
          message += (char)LoRa.read();
      }
      Serial.print(" | RSSI: ");
      Serial.print(LoRa.packetRssi());
      Serial.print("\n");
  }
  if (message) {
    Serial.println(message);
    writeToFile(message);
    
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
