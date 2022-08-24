#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <cmath>
#include <string>
#include <sstream>

#include "sensors/altimeter.cpp"
#include "sensors/IMUSensor.cpp"
#include "headers/GPS.h"
#include "communication/rocket.cpp"

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

String formatMessage(String gpsTime, String gpsLat, String gpsLng, String gpsSpeed, String gpsAlt, String imuAccX, String imuAccY, String imuAccZ, String imuOmegaX, String imuOmegaY, String imuOmegaZ, float *imuQuat, String barAlt) {
  
  
  
  String formattedDataSend = "GPS - Time: " + gpsTime + " Lat: " + gpsLat + " Lng: " + gpsLng + " Speed: " + gpsSpeed + " Alt: " + gpsAlt;
  formattedDataSend += " IMU - Acc: " + imuAccX + "," + imuAccY + "," + imuAccZ + " Omega: " + imuOmegaX + "," + imuOmegaY + "," + imuOmegaZ + "Quat: " + String(imuQuat[0]) + "," + String(imuQuat[1]) + "," + String(imuQuat[2]) + String(imuQuat[3]); // +quat
  formattedDataSend += " Bar - Alt: " + barAlt;
  return formattedDataSend;
}

String formatSDMessage(double gpsLat, double gpsLong, uint32_t gpsTime, float accel, float alt) {
  String gpsLatString = String(gpsLat);
  String gpsLongString = String(gpsLong);
  String gpsTimeString = String(gpsTime);
  String accelString = String(accel);
  String altString = String(alt);
  String formattedSDMessage = String(gpsTimeString + "," + gpsLatString + "," + gpsLongString + "," + accelString + "," + altString);
  return formattedSDMessage;
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

  String newSetupLine = String("------gpsTime, gpsLat, gpsLng, gpsSpeed, gpsAlt | imuAccX, imuAccY, imuAccZ, imuOmegaX, imuOmegaY, imuOmegaZ, imuQuat | barAlt------");
  writeToFile(newSetupLine);

  // sensors
  Serial.println("Initialising sensors...");
  altSensor = AltimeterSensor();
  altSensor.setup();
  Serial.println("Initialised altitude sensor.");
  imuSensor = IMUSensor();
  imuSensor.setup();
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
  float accel = imuSensor.getAcceleration();
  float alt = altSensor.getAltitude();
  double gpsLat = gps.getLat();
  double gpsLong = gps.getLng();
  uint32_t gpsTime = gps.getTime();

  // gps data
  u_int32_t gpsTime = gps.getTime();
  float gpsLat = gps.getLat();
  float gpsLng = gps.getLng();
  float gpsSpeed = gps.getSpeed();
  float gpsAlt = gps.getAlt();

  // imu data
  float imuAccX = imuSensor.getAccX();
  float imuAccY = imuSensor.getAccY();
  float imuAccZ = imuSensor.getAccZ();
  float imuOmegaX = imuSensor.getOmegaX();
  float imuOmegaY = imuSensor.getOmegaY();
  float imuOmegaZ = imuSensor.getOmegaZ();
  float[4] imuQuat; 
  imuSensor.getQuat(&imuQuat[0]);

  // bar data
  float barAlt = altSensor.getAltitude();

  String sensorDataSend = formatMessage(String(gpsTime), String(gpsLat), String(gpsLng), String(gpsSpeed), String(gpsAlt), 
                                        String(imuAccX), String(imuAccY), String(imuAccZ), String(imuOmegaX), String(imuOmegaY), String(imuOmegaZ), &imuQuat, String(barAlt));
  String sensorDataSave = formatSDMessage(String(gpsTime), String(gpsLat), String(gpsLng), String(gpsSpeed), String(gpsAlt), 
                                        String(imuAccX), String(imuAccY), String(imuAccZ), String(imuOmegaX), String(imuOmegaY), String(imuOmegaZ), &imuQuat, String(barAlt));


  Serial.println(sensorDataSend);
  rocket.sendMessage(sensorDataSend);

  digitalWrite(TRANSMIT_LED, HIGH);
  delay(200);
  digitalWrite(TRANSMIT_LED, LOW);

  writeToFile(sensorDataSave);
}

