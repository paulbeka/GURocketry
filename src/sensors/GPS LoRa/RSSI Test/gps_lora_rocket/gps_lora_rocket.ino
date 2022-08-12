// RSSI Test w/ GPS - Rocket
// Cameron Fraser | GUR Software | 5/8/22

// Libraries
#include <LoRa.h>             // LoRa transmission
#include <TinyGPS++.h>        // gps library
#include <SoftwareSerial.h>   // interface with gps

// gps pins
#define RXPin 0
#define TXPin 1

// LoRa pins
#define RFM95_CS 10
#define RFM95_RST 22
#define RFM95_INT 23

// LoRa Freq
#define LoRaFreq 433E6 // operating on 433MHz

// Object declarations
SoftwareSerial ss(RXPin, TXPin); // Serial connection to the GPS device
TinyGPSPlus gps;                 // Tiny GPS++ object

// Global variable declarations
char gpsData[36];  // HHMMSSCC_00000.000000_00000.000000
int messageNum = 0;


void setup() {
  // Serial init
  Serial.begin(9600);
  while (!Serial);
  Serial.println("GUR | RSSI Test - Rocket");


  // SoftwareSerial init
  Serial.println("Intialising Software Serial...");
  ss.begin(9600);
  if (!ss) {
    Serial.println("Error initialising Software Serial!");
    while (1);
  }
  Serial.println("Software Serial initialised.\n");


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
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      if (gps.location.isValid()){
        if (gps.location.isUpdated()){
          // get gpsData
          sprintf(gpsData, "%lu_%5.6f_%5.6f", gps.time.value(), gps.location.lat(), gps.location.lng());
          Serial.print("GPS Updated: "); Serial.println(gpsData);


          // send via LoRa
          LoRa.beginPacket();
          LoRa.print("#");
          LoRa.print(messageNum);
          LoRa.print(" ");
          LoRa.print(gpsData);
          LoRa.endPacket();
          messageNum++;
          Serial.print(gpsData); Serial.println(" successfully sent via LoRa.");
        }
      }
    }
  }
}
