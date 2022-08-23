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
  String formattedString = String("Time: " + gpsTimeString + " | Lat: " + gpsLatString + " Long: " + gpsLongString);
  formattedString += String(" | Acceleration: " + String(accel) + " Altitude: " + String(alt));
  return formattedString;
}

String formatSDMessage(double gpsLat, double gpsLong, uint32_t gpsTime, float accel, float alt) {
  String gpsTimeString = String(gpsTime);
  String formattedSDMessage = String(gpsTimeString + "," + String(gpsLat) + "," + String(gpsLong) + "," + String(accel) + "," + String(alt));
  return formattedSDMessage;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRANSMIT_LED, OUTPUT);
  Serial.begin(115200);
  
  // LoRa
  rocket = Rocket();
  rocket.setup();

  // SD
  while (!SD.begin(CS_PIN)) {
    Serial.println("Error initialising SD card");
    delay(300);
  }
  Serial.println("Initialised SD card");

  String newSetupLine = String("------time,lat,long,accel,alt------");
  writeToFile(newSetupLine);

  // sensors
  Serial.println("Initializing sensors...");
  altSensor = AltimeterSensor();
  altSensor.setup();
  Serial.println("Initialized altitude sensor.");
  imuSensor = IMUSensor();
  imuSensor.setup();
  Serial.println("Initialized IMU sensor.");
  gps = GPS();
  gps.setup();
  Serial.println("Initialized GPS sensor.");

  // Setup success light
  digitalWrite(TRANSMIT_LED, HIGH);
  delay(3000);
  digitalWrite(TRANSMIT_LED, LOW);
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

