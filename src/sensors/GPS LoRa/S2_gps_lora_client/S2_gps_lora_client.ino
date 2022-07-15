// GPS Module Client w/ LoRa Send & SD Card Save
// Cameron Fraser | GUR Software | 14/3/22

/*
 * Message counting works as follows:
 * Message #0 is the fileName being sent to the server so the data is consistent
 * Every subsequent message is gpsData
 */

// Libraries
#include <LoRa.h>             // LoRa transmission
#include <TinyGPS++.h>        // gps library
#include <SoftwareSerial.h>   // interface with gps
#include <SD.h>               // SD


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


// File declarations 
File sdRoot;
File sdFile;


// Global variable declarations
String gpsLat;
String gpsLng;
char gpsData[36];  // HHMMSSCC_00000.000000_00000.000000
char fileName[28]; // DDMMYY_HHMMSSCC_gps_data.txt
int messageNum = 0;
int fileCount = 0;
String fileLog[50]; // make a check that 50 files is never exceeded


void setup() {
  // Serial init
  Serial.begin(9600);
  while (!Serial);
  Serial.println("GUR | LoRa GPS Client");


  // SoftwareSerial init
  Serial.println("Intialising Software Serial...");
  ss.begin(9600);
  if (!ss) {
    Serial.println("Error initialising Software Serial!");
    while (1);
  }
  Serial.println("Software Serial initialised.\n");
  // maybe replace with: while (!ss);


  // LoRa init
  Serial.println("Initialising LoRa...");
  LoRa.setPins(RFM95_CS, RFM95_RST, RFM95_INT);
  if (!LoRa.begin(LoRaFreq)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init successful.\n");
  LoRa.setTxPower(23); // increase power and therefore range by +20db (5-23 available)


  // SD card init
  Serial.println("Initialising SD Card...");
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Error initialising SD card!");
    while (1);
  }
  Serial.println("SD card init successful.\n");

  // Display files on SD card
  Serial.println("Fetching files on SD card...");

  File sdRoot = SD.open("/");
  if (!sdRoot){
    Serial.println("Error opening root directory!");
    while (1);
  }
  printDirectory(sdRoot, 0);
  
   // read any files?
  bool readFiles = inputYesNo("Do you want to read any files?");
  if (readFiles){
    while (!readFileData());
  }

//  // format SD?
//  bool clearSD = inputYesNo("Do you want to clear SD card data?");
//  if (clearSD){
//    bool clearSDCheck = inputYesNo("Are you sure?");
//    if (clearSDCheck) {
//      Serial.println("Wiping SD card...");
//      wipeSDCard(sdRoot);
//      Serial.println("SD card Formatted.");
//    }   
//  }
  sdRoot.close();


  // GPS lock/init
  Serial.println("\nWaiting for GPS Lock...");
  while(!gps.location.isValid()){
    while(ss.available() > 0){
      if (gps.encode(ss.read())){
        if (gps.location.isValid()){
          gpsLat = gps.location.lat();
          gpsLng = gps.location.lng();
          Serial.print("GPS Locked: "); Serial.print(gpsLat); Serial.print(" "); Serial.println(gpsLng);
          sprintf(fileName, "%lu_%lu_gpsData.txt", gps.date.value(), gps.time.value());
          Serial.print("File name: "); Serial.println(fileName);
          break;
        }
      }
    }
  }


  // send fileName to server
  Serial.println("\nSending file name to server");
  LoRa.beginPacket();
  LoRa.print("#");
  LoRa.print(messageNum);
  LoRa.print(" ");
  LoRa.print(fileName);
  LoRa.endPacket();
  messageNum++;
  Serial.print(fileName); Serial.println(" sent to server.\n");
  
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


          // save to SD
          Serial.print("Writing "); Serial.print(gpsData); Serial.println(" to SD card...");
          sdFile = SD.open(fileName, FILE_WRITE);
          if (sdFile) {
            sdFile.println(gpsData);
            Serial.println("Write to SD card successful.");
          }
          else {
            Serial.print("Error opening "); Serial.print(fileName); Serial.println(" on SD card!");
          }
          sdFile.close();
        }
      }
    }
  }
}

bool readFileData(){
  // unedited version from: https://www.hackster.io/Neutrino-1/littlefs-read-write-delete-using-esp8266-and-arduino-ide-867180
  // returns true when valid file given 
  // doesn't work if data is in a folder

  Serial.println("Please enter the number of the file you want to view... (x to exit choice)");
  
  while (!Serial.available()); // wait for input
  char requestedFileChar = Serial.read();
  Serial.println(requestedFileChar);
  
  if (tolower(requestedFileChar) == 'x') {return true;}

  int requestedFileCharAscii = (int)requestedFileChar;
  if (requestedFileCharAscii < 48 || requestedFileCharAscii > 57) {
    Serial.print(requestedFileChar); Serial.println(" is not a valid number!");
    return false;
  }
  
  int requestedFileNo = requestedFileChar - '0';
  String requestedFileNameStr = fileLog[requestedFileNo];    // should I get reference or value?
  
  Serial.print("debug | input no: "); Serial.print(requestedFileNo); Serial.print(" file name: "); Serial.println(requestedFileNameStr);

  char requestedFileNameArr[32]; 
  requestedFileNameStr.toCharArray(requestedFileNameArr, 32);

  Serial.print("debug | requestedFileNameArr: "); Serial.println(requestedFileNameArr);
  
  File requestedFile = SD.open(requestedFileNameArr, FILE_READ);
  
  if (requestedFile){
    Serial.print(requestedFileNameArr); Serial.println(":");
    while(requestedFile.available()){
        Serial.print((char)requestedFile.read());
    }
    requestedFile.close();
    Serial.println("");
    return true;
  }
  else{
    Serial.print(requestedFileNo); Serial.println(" is not a valid choice!\n");
    return false;
  }
}

bool inputYesNo(String question){
  // gets y/n input from Serial, returns: true = y, false = n
  
  while (1) {
    Serial.print(question); Serial.println(" (y/n)");
    while(!Serial.available());
    char input = tolower(Serial.read());
    Serial.println(input);
    if (input == 'y'){
      return true;
    }
    else if (input == 'n'){
      return false;
    }
    else {
      Serial.print(input); Serial.println(" is not a valid option!");
    }
  }
}

void printDirectory(File dir, int numTabs) {
  // from: https://www.arduino.cc/en/Tutorial/LibraryExamples/Listfiles#code

  while (true) {
    File entry = dir.openNextFile();
    
    if (!entry) {
      // no more files
      break;
    }
    
    for (int i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }

    if (entry.isDirectory()) {
      Serial.print(entry.name());
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } 
    else {
      // files have sizes, directories do not
      Serial.print("#"); 
      Serial.print(fileCount); 
      Serial.print(" ");
      Serial.print(entry.name());
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);

      // store entry name in fileLog
      fileLog[fileCount] = entry.name();
      
      fileCount++;
    }
    entry.close();
  }
}

// MAY NOT WORK
void wipeSDCard(File dir) {

  while (1){
    File entry = dir.openNextFile();

    if (!entry){
      // no more files in directory
     break;
    }

    if (entry.name() == "System Volume Information"){
      // dont want to crucial files - may brick SD card
      continue;
    }
    
    if (entry.isDirectory()){
      wipeSDCard(entry); // recursive call to delete files inside directory
      SD.rmdir(entry.name()); // delete directory once all files are deleted
    }
    else {
      Serial.print("Deleting "); Serial.print(entry.name()); Serial.println("...");
      SD.remove(entry.name());
    }
    entry.close();
  }
}


/*
 * File
 * Dir
 *    File
 *    Dir
 *      File
 *      File
 *    Dir
 *      File
 *      File
 *      Dir
 *        File
 *      Dir
 * File
 */
