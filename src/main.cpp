#include <Arduino.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <cmath>
#include <string>
#include <sstream>

#include <Matrix.h>
#include "../lib/KalmanMath/KalmanMath.h"
#include "sensors/altimeter.cpp"
#include "sensors/IMUSensor.cpp"
#include "communication/rocket.cpp"

using std::string; using std::vector;

char filename[9] = "data.txt";
AltimeterSensor altSensor;
IMUSensor imuSensor;

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

String formattedMessage(double accel, double alt) {
  String formattedString = String("Acceleration: " + String(accel) + " Altitude: " + String(alt));
  return formattedString;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  // LoRa
  rocket = Rocket();
  rocket.setup();

  // SD
  // if (!SD.begin(BUILTIN_SDCARD)) {
  //   Serial.println("Error initialising SD card");
  //   while (1);
  // }
  String line = String("-------------------");
  // writeToFile(line);

  // sensors
  Serial.println("Initializing sensors...");
  altSensor = AltimeterSensor();
  altSensor.setup();
  Serial.println("Initialized altitude sensor.");
  imuSensor = IMUSensor();
  imuSensor.setup();
  Serial.println("Initialized IMU sensor.");

  // Kalman
  calculator = KalmanMath();
  currentState = {.state = Matrix(3,1), .covariance = Matrix(3,3)};
}

void loop() {
  float accel = imuSensor.getAcceleration();
  float alt = altSensor.getAltitude();
  // Matrix sensorValues = getSensorReadings();
  // H = getHValues(sensorValues);
  // currentState = calculator.kalmanIteration(currentState, sensorValues, H);
  String message = formattedMessage(accel, alt);
  Serial.println(message);
  // writeToFile(message);
  rocket.sendMessage(message);
}

