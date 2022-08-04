#include <Arduino.h>
#include "../lib/KalmanMath/KalmanMath.h"

#include "sensors/altimeter.cpp"
#include "sensors/IMU.cpp"
#include "sensors/GPS_parseData.cpp"
#include <Matrix.h>
#include <cmath>


Adafruit_MPL3115A2 altSensor;
IMU imu;
GPSClass GPSSensor;

KalmanMath calculator;
Matrix H;
StateAndCovariance& currentState;

Matrix getSensorReadings() {
  std::vector<double> temp;
  temp.push_back(altSensor.getAltitude());
  temp.push_back(imu.getAcceleration());
  std::vector<std::vector<double>> initialReadings;
  initialReadings.push_back(temp);
  Matrix initialMatrix = Matrix(initialReadings, 2, 1);
  return initialMatrix;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  // sensors
  altSensor = Adafruit_MPL3115A2();
  imu = IMU();  
  GPSSensor = GPSClass();

  calculator = KalmanMath();

  std::vector<std::vector<double>> initialH(2, std::vector<double>(3, 0));
  initialH[0][0] = 1;
  initialH[1][2] = 1;
  H = Matrix(initialH, 3, 2);

  // initial SC
  currentState = {
    
  };
  
}

void loop() {

  Matrix sensorValues =  getSensorReadings();
  currentState = calculator.kalmanIteration(currentState, sensorValues, H);

}