#include <Arduino.h>
#include <SD.h>
#include <cmath>
#include <string>
#include <sstream>

#include <Matrix.h>
#include "../lib/KalmanMath/KalmanMath.h"
#include "sensors/altimeter.cpp"
#include "sensors/IMUSensor.cpp"
#include "headers/GPS.h"
#include "communication/rocket.cpp"

using std::string; using std::vector;

char filename[9] = "data.txt";
Adafruit_MPL3115A2 altSensor;
IMUSensor imuSensor;
GPS gps;

Rocket rocket;
KalmanMath calculator;
Matrix H;
StateAndCovariance currentState;

void writeToFile(String message) {
  File sdFile = SD.open(filename, FILE_WRITE);
  sdFile.println(message);
  sdFile.close();
}

Matrix getSensorReadings() {
  std::vector<double> temp;
  temp.push_back(altSensor.getAltitude());
  temp.push_back(imuSensor.getAcceleration());
  std::vector<std::vector<double>> initialReadings;
  initialReadings.push_back(temp);
  Matrix initialMatrix = Matrix(initialReadings, 2, 1);
  return initialMatrix;
}

Matrix getHValues(Matrix sensorValues) {
  std::vector<std::vector<double>> HList = {{0,0,0}, {0,0,0}};
  if (sensorValues(0,0) != 0) {
    HList[0][0] = 1;
  }
  if (sensorValues(0,1) != 0) {
    HList[1][2] = 1;
  }
  return Matrix(HList, 2, 3);
} 

String formattedMessage(double gpsLat, double gpsLong, uint32_t gpsTime) {
  String gpsTimeString = String(gpsTime);
  String gpsLatString = String(gpsLat);
  String gpsLongString = String(gpsLong);
  String formattedString = String("Time: " + gpsTimeString + " | Lat: " + gpsLatString + " Long: " + gpsLongString);
  return formattedString;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  // LoRa
  rocket = Rocket();
  rocket.setup();

  // SD
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Error initialising SD card");
    while (1);
  }
  String line = String("-------------------");
  writeToFile(line);

  // sensors
  Serial.println("Initializing sensors...");
  altSensor = Adafruit_MPL3115A2();
  Serial.println("Initialized altitude sensor.");
  imuSensor = IMUSensor();
  imuSensor.setup();
  Serial.println("Initialized IMU sensor.");
  gps = GPS();
  gps.setup();
  Serial.println("Initialized GPS sensor.");

  // Kalman
  calculator = KalmanMath();
  currentState = {.state = Matrix(3,1), .covariance = Matrix(3,3)};
}

void loop() {
  Matrix sensorValues = getSensorReadings();
  H = getHValues(sensorValues);
  currentState = calculator.kalmanIteration(currentState, sensorValues, H);
  double gpsLat = gps.getLat();
  double gpsLong = gps.getLong();
  uint32_t gpsTime = gps.getTime();
  String message = formattedMessage(gpsLat, gpsLong, gpsTime);
  writeToFile(message);
  rocket.sendMessage(message);
  
}

