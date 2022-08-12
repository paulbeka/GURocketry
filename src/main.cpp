#include <Arduino.h>
#include "../lib/KalmanMath/KalmanMath.h"

#include "sensors/altimeter.cpp"
#include "sensors/IMUSensor.cpp"
#include "sensors/GPS_parseData.cpp"
#include <Matrix.h>
#include <cmath>


Adafruit_MPL3115A2 altSensor;
IMUSensor imuSensor;
GPSClass GPSSensor;

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
  
  // sensors
  Serial.println("Initializing sensors...");
  altSensor = Adafruit_MPL3115A2();
  Serial.println("Initialized altitude senror.");
  imuSensor = IMUSensor();
  imuSensor.setup();
  Serial.println("Initialized IMU senror.");
  GPSSensor = GPSClass();
  Serial.println("Initialized GPS senror.");

  calculator = KalmanMath();

}

void loop() {

  Matrix sensorValues =  getSensorReadings();
  //currentState = calculator.kalmanIteration(currentState, sensorValues, H);
}