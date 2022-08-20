#include <Arduino.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
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
Adafruit_GPS GPS;

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

String formattedMessage(double gpsLat, double gpsLong, uint32_t gpsTime, double accel, double alt) {
  String gpsTimeString = String(gpsTime);
  String gpsLatString = String(gpsLat);
  String gpsLongString = String(gpsLong);
  String formattedString = String("Time: " + gpsTimeString + " | Lat: " + gpsLatString + " Long: " + gpsLongString);
  formattedString += String("| Acceleration: " + String(accel) + " Altitude: " + String(alt));
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

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  Serial.println("Initialized GPS sensor.");

  // Kalman
  calculator = KalmanMath();
  currentState = {.state = Matrix(3,1), .covariance = Matrix(3,3)};
}

void loop() {
  GPS.read();
  float accel = imuSensor.getAcceleration();
  float alt = altSensor.getAltitude();
  double gpsLat = 0;
  double gpsLong = 0;
  uint32_t gpsTime = 0;
  // Matrix sensorValues = getSensorReadings();
  // H = getHValues(sensorValues);
  // currentState = calculator.kalmanIteration(currentState, sensorValues, H);
  if (GPS.newNMEAreceived()) {
    GPS.parse(GPS.lastNMEA());
  }
  if (GPS.fix) {
    gpsLat = GPS.latitude;
    gpsLong = GPS.longitude;
    gpsTime = GPS.minute;
  }
  String message = formattedMessage(gpsLat, gpsLong, gpsTime, accel, alt);
  // String message2 = String("ACCEL: " + String(accel) + " | ALT: " + String(alt));
  Serial.println(message);
  // writeToFile(message);
  rocket.sendMessage(message);
}

