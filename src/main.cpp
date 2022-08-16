#include <Arduino.h>
#include "../lib/KalmanMath/KalmanMath.h"

#include "sensors/altimeter.cpp"
#include "sensors/IMUSensor.cpp"
#include "headers/GPS.h"
#include "communication/rocket.cpp"
#include <Matrix.h>
#include <cmath>
#include <string>

using std::string; using std::vector;

Adafruit_MPL3115A2 altSensor;
IMUSensor imuSensor;
GPS gps;

Rocket rocket;
KalmanMath calculator;
Matrix H;
//StateAndCovariance& currentState;

Matrix getSensorReadings() {
  std::vector<double> temp;
  temp.push_back(altSensor.getAltitude());
  temp.push_back(imuSensor.getAcceleration());
  std::vector<std::vector<double>> initialReadings;
  initialReadings.push_back(temp);
  Matrix initialMatrix = Matrix(initialReadings, 2, 1);
  return initialMatrix;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  // LoRa
  rocket = Rocket();
  rocket.setup();

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

  calculator = KalmanMath();
}

void loop() {

  Matrix sensorValues =  getSensorReadings();
  //currentState = calculator.kalmanIteration(currentState, sensorValues, H);
}