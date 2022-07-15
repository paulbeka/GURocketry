// GPS Module Server w/ LoRa Recieve & SD Card Save
// Cameron Fraser | GUR Software | 14/3/22

// Turn server on first then client!!

// Libraries
#include <SPI.h>
#include <SD.h>
#include <LoRa.h>

// LoRa pins
#define RFM95_CS 10
#define RFM95_RST 22
#define RFM95_INT 23

// LoRa Freq
#define LoRaFreq 433E6 // operating on 433MHz

File sdFile;

// Global Variable Declarations
char fileNameArr[28]; // DDMMYY_HHMMSSCC_gps_data.txt
String fileNameStr = "";
//String fileName; // DDMMYY_HHMMSSCC_gps_data.txt

void setup() {
  // Serial init
  Serial.begin(9600);
  while (!Serial);
  Serial.println("GUR | LoRa GPS Server");

  // SD card init
  Serial.println("Initialising SD Card...");
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Error initialising SD card");
    while (1);
  }
  Serial.println("SD card init successful.\n");

  // LoRa init
  Serial.println("Initialising LoRa...");
  LoRa.setPins(RFM95_CS, RFM95_RST, RFM95_INT);
  if (!LoRa.begin(LoRaFreq)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init successful.\n");
  LoRa.setTxPower(20); // increase power and therefore range by +20db (5-23 available)

  // Recieve fileName from client
  Serial.println("Awaiting file name from client...");
  int charCounter = 0;
  bool fileNameRecieved = false;
  while (!fileNameRecieved){
    int packetSize = LoRa.parsePacket();

    if (packetSize) {
      Serial.print("File name recieved: ");
      fileNameRecieved = true;
      
      while (LoRa.available()){
        char incoming = (char)LoRa.read(); 
        Serial.print(incoming);

        if (charCounter >= 3) {
          fileNameStr += incoming;
        } 

        charCounter++;
      }

      Serial.print(" | RSSI: ");
      Serial.print(LoRa.packetRssi());
      Serial.print("\n");
    }
  }

  
  if (fileNameStr.charAt(sizeof(fileNameStr) - 1) != 't') {
    fileNameStr = "gpsData.txt";
  }
  
  // convert recieved fileName string to char array
  fileNameStr.toCharArray(fileNameArr, sizeof(fileNameArr));
  Serial.print("fileNameStr: "); Serial.println(fileNameStr);
  Serial.print("fileNameArr: "); Serial.println(fileNameArr);
  Serial.println("Set up complete.\n");
}

void loop() {
  int charCounter = 0;
  String gpsData = ""; // HHMMSSCC_00000.000000_00000.000000
  
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

        // store gpsData except "[num]# "
        if (charCounter >= 3) {
          gpsData += incoming;
        }
        
        charCounter++;
      }

      Serial.print(" | RSSI: ");
      Serial.print(LoRa.packetRssi());
      Serial.print("\n");

      // save recieved gpsData to SD card
      sdFile = SD.open(fileNameArr, FILE_WRITE);
      sdFile.print(gpsData);
      Serial.print(fileNameArr); Serial.print(" "); Serial.println(gpsData);
      sdFile.print("\n");
      sdFile.close();
   }
}
