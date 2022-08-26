/* GU Rocketry - Saltire 2 | Flight Computer | 25/08/22 */
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
unsigned long timeStart;
unsigned long timeElapsed;

AltimeterSensor altSensor;
IMUSensor imuSensor;
GPS gps;

Rocket rocket;

void writeToFile(String message) {
  File sdFile = SD.open(filename, FILE_WRITE);
  sdFile.println(message);
  sdFile.close();
}

String formatMessage(String timeElapsed, String gpsTime, String gpsLat, String gpsLng, String gpsSpeed, String gpsAlt, String imuData, String barAlt) {
  String formattedDataSend = "FCC - Millis: " + timeElapsed + " | GPS - Time: " + gpsTime + " Lat: " + gpsLat + " Lng: " + gpsLng + " Speed: " + gpsSpeed + " Alt: " + gpsAlt;
  formattedDataSend += " | IMU - " + imuData;
  formattedDataSend += " | BAR - Alt: " + barAlt;
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
  Serial.println();

  // SD
  Serial.println("Initialising SD card...");
  while (!SD.begin(CS_PIN)) {
    Serial.println("Error initialising SD card!");
    delay(300);
  }
  Serial.println("Initialised SD card.\n");

  String newSetupLine = String("------fccMillis, gpsTime, gpsLat, gpsLng, gpsSpeed, gpsAlt | imuAccX, imuAccY, imuAccZ, imuOmegaX, imuOmegaY, imuOmegaZ, imuQuatW, imuQuatX, imuQuatY, imuQuatZ | barAlt------");
  writeToFile(newSetupLine);

  // Sensors
  Serial.println("Initialising sensors...");
  gps = GPS();
  gps.setup();
  imuSensor = IMUSensor();
  imuSensor.setup();
  imuSensor.getDetails();
  altSensor = AltimeterSensor();
  altSensor.setup();
  Serial.println("Sensors initialised.\n");

  // Setup success light
  digitalWrite(TRANSMIT_LED, HIGH);
  delay(3000);
  digitalWrite(TRANSMIT_LED, LOW);

  Serial.println("Set up complete.\n");
  timeStart = millis();
}

void loop() {
  timeElapsed = millis() - timeStart;

  // GPS data
  uint32_t gpsTime = gps.getTime();
  float gpsLat = gps.getLat();
  float gpsLng = gps.getLng();
  float gpsSpeed = gps.getSpeed();
  float gpsAlt = gps.getAlt();

  // IMU data
  String imuData = imuSensor.getAllSensorData();
  if (timeElapsed <= 60000) {
    imuSensor.getCalVals();
  }

  // BAR data
  float barAlt = altSensor.getAltitude();

  // Concatenate sensor data
  String sensorData = formatMessage(String(timeElapsed), String(gpsTime), String(gpsLat), String(gpsLng), String(gpsSpeed), String(gpsAlt), imuData, String(barAlt));

  // Send and save sensor data
  Serial.println(sensorData);
  rocket.sendMessage(sensorData);

  digitalWrite(TRANSMIT_LED, HIGH);
  delay(100);
  digitalWrite(TRANSMIT_LED, LOW);

  writeToFile(sensorData);
}