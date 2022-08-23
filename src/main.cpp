#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <cmath>
#include <string>
#include <sstream>

#include <Matrix.h>
#include "sensors/altimeter.cpp"
#include "sensors/IMUSensor.cpp"
#include "communication/rocket.cpp"

#define CS_PIN 10 
#define TRANSMIT_LED 21

Rocket rocket;

char filename[9] = "data.txt";
AltimeterSensor altSensor;
IMUSensor imuSensor;

void writeToFile(String message) {
  File sdFile = SD.open(filename, FILE_WRITE);
  sdFile.println(message);
  sdFile.close();
}

String formattedMessage(double accel, double alt) {
  String formattedString = String("Acceleration: " + String(accel) + " Altitude: " + String(alt));
  return formattedString;
}

String formatWrittenMessage(double accel, double alt) {
  String formattedWrittenString = String(String(accel) + "," + String(alt));
  return formattedWrittenString;
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

  String newSetupLine = String("-------Acceleration,Altitude---------");
  writeToFile(newSetupLine);

  // sensors
  Serial.println("Initializing sensors...");
  altSensor = AltimeterSensor();
  altSensor.setup();
  Serial.println("Initialized altitude sensor.");
  imuSensor = IMUSensor();
  imuSensor.setup();
  Serial.println("Initialized IMU sensor.");

  digitalWrite(TRANSMIT_LED, HIGH);
  delay(3000);
  digitalWrite(TRANSMIT_LED, LOW);
}

void loop() {
  float accel = imuSensor.getAcceleration();
  float alt = altSensor.getAltitude();

  String message = formattedMessage(accel, alt);
  String sdString = formatWrittenMessage(accel, alt);

  Serial.println(message);
  rocket.sendMessage(message);
  writeToFile(sdString);

  digitalWrite(TRANSMIT_LED, HIGH);
  delay(200);
  digitalWrite(TRANSMIT_LED, LOW);
}

