#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "../headers/GPS.h"

#define GPSSerial Serial1
#define RX_PIN 0
#define TX_PIN 1

Adafruit_GPS adafruit_gps(&GPSSerial);
// SoftwareSerial ss(TX_PIN, RX_PIN);
// Adafruit_GPS adafruit_gps(&ss);

GPS::GPS() { 
}

void GPS::setup() {
    Serial.println("Initialising GPS sensor");
    adafruit_gps.begin(115200);
    adafruit_gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    adafruit_gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    Serial.println("Initialised sensor");
    while (!adafruit_gps.fix) {
        Serial.println("No fix found...");
        refreshNMEA();
        delay(2000);
    }
    Serial.println("Fix found!");
}

float GPS::getLong() {
    refreshNMEA();
    return adafruit_gps.longitude;
}

float GPS::getLat() {
    refreshNMEA();
    return adafruit_gps.longitude;
}

String GPS::getTime() {
    refreshNMEA();
    uint8_t hour = adafruit_gps.hour;
    uint8_t minute = adafruit_gps.minute;
    uint8_t seconds = adafruit_gps.seconds;
    return String(String(hour) + ":" + String(minute) + ":" + String(seconds));
}

bool GPS::refreshNMEA() {
    adafruit_gps.read();
    if (adafruit_gps.newNMEAreceived()) {
        if (adafruit_gps.parse(adafruit_gps.lastNMEA())) 
            return true;
    }
    return false;
}

bool GPS::isFixFound() {
    return adafruit_gps.fix;
}