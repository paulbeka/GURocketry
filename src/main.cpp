#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <cmath>
#include <string>
#include <sstream>
#include <array>

#include "sensors/altimeter.cpp"
#include "sensors/IMUSensor.cpp"
#include "headers/GPS.h"
#include "communication/rocket.cpp"

using std::array;

#define CS_PIN 10 
#define TRANSMIT_LED 21

char filename[9] = "data.txt";
AltimeterSensor altSensor;
IMUSensor imuSensor;

GPS gps;
Rocket rocket;

void writeToFile(String message) {
  File sdFile = SD.open(filename, FILE_WRITE);
  sdFile.println(message);
  sdFile.close();
}

String formatMessage(String gpsTime, String gpsLat, String gpsLng, String gpsSpeed, String gpsAlt, String imuAccX, String imuAccY, String imuAccZ, String imuOmegaX, String imuOmegaY, String imuOmegaZ, std::array<double, 4> imuQuat, String barAlt) {
  String formattedDataSend = "GPS - Time: " + gpsTime + " Lat: " + gpsLat + " Lng: " + gpsLng + " Speed: " + gpsSpeed + " Alt: " + gpsAlt;
  formattedDataSend += " | IMU - Acc: " + imuAccX + "," + imuAccY + "," + imuAccZ + " Omega: " + imuOmegaX + "," + imuOmegaY + "," + imuOmegaZ + " Quat: (" + String(imuQuat[0]) + "," + String(imuQuat[1]) + "," + String(imuQuat[2]) + "," + String(imuQuat[3]) +")";
  formattedDataSend += " | Bar - Alt: " + barAlt;
  return formattedDataSend;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRANSMIT_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("GU Rocketry - Saltire 2 | Flight Computer\n");
  
  // LoRa
  rocket = Rocket();
  rocket.setup();

  // SD
  while (!SD.begin(CS_PIN)) {
    Serial.println("Error initialising SD card!");
    delay(300);
  }
  Serial.println("Initialised SD card.");

  // String newSetupLine = String("------gpsTime, gpsLat, gpsLng, gpsSpeed, gpsAlt | imuAccX, imuAccY, imuAccZ, imuOmegaX, imuOmegaY, imuOmegaZ, imuQuat | barAlt------");
  // writeToFile(newSetupLine);

  // sensors
  Serial.println("Initialising sensors...");
  altSensor = AltimeterSensor();
  altSensor.setup();
  Serial.println("Initialised altitude sensor.");
  imuSensor = IMUSensor();
  imuSensor.setup();
  imuSensor.getDetails(); // imu debug
  imuSensor.getCal(); // imu debug
  Serial.println("Initialised IMU sensor.");
  gps = GPS();
  gps.setup();
  Serial.println("Initialised GPS sensor.");
  Serial.println("Sensors initialised.");

  // Setup success light
  digitalWrite(TRANSMIT_LED, HIGH);
  delay(3000);
  digitalWrite(TRANSMIT_LED, LOW);

  Serial.println("Set up complete.\n");
}

void loop() {

  // gps data
  uint32_t gpsTime = gps.getTime();
  float gpsLat = gps.getLat();
  float gpsLng = gps.getLng();
  float gpsSpeed = gps.getSpeed();
  float gpsAlt = gps.getAlt();

  // imu data
  // float imuAccX = imuSensor.getAccX();
  // float imuAccY = imuSensor.getAccY();
  // float imuAccZ = imuSensor.getAccZ();
  // float imuOmegaX = imuSensor.getOmegaX();
  // float imuOmegaY = imuSensor.getOmegaY();
  // float imuOmegaZ = imuSensor.getOmegaZ();
  // std::array<double, 4> imuQuat = imuSensor.getQuat();
  imuSensor.getAllSensorData(); // imu debug

  // bar data
  float barAlt = altSensor.getAltitude();

  // String sensorData = formatMessage(String(gpsTime), String(gpsLat), String(gpsLng), String(gpsSpeed), String(gpsAlt), 
  //                                   String(imuAccX), String(imuAccY), String(imuAccZ), String(imuOmegaX), String(imuOmegaY), String(imuOmegaZ), imuQuat, 
  //                                   String(barAlt));


  // Serial.println(sensorData);
  // rocket.sendMessage(sensorData);

  // digitalWrite(TRANSMIT_LED, HIGH);
  // delay(200);
  // digitalWrite(TRANSMIT_LED, LOW);

  // writeToFile(sensorData);
}

