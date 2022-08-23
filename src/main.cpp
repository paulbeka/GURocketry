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

String formattedMessage(double gpsLat, double gpsLong, uint32_t gpsTime, float accel, float alt) {
  String gpsLatString = String(gpsLat);
  String gpsLongString = String(gpsLong);
  String gpsTimeString = String(gpsTime);
  String accelString = String(accel);
  String altString = String(alt);
  String formattedString = String("Time: " + gpsTimeString + " | Lat: " + gpsLatString + " Long: " + gpsLongString);
  formattedString += String(" | Acceleration: " + accelString + " Altitude: " + altString);
  return formattedString;
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

  String newSetupLine = String("------time,lat,long,accel,alt------");
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
  double gpsLong = gps.getLong();
  uint32_t gpsTime = gps.getTime();

  String message = formattedMessage(gpsLat, gpsLong, gpsTime, accel, alt);
  String sdString = formatSDMessage(gpsLat, gpsLong, gpsTime, accel, alt);

  Serial.println(message);
  rocket.sendMessage(message);

  digitalWrite(TRANSMIT_LED, HIGH);
  delay(200);
  digitalWrite(TRANSMIT_LED, LOW);

  writeToFile(sdString);
}

