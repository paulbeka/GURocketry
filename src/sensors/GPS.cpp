#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "../headers/GPS.h"

#define RXPin 0
#define TXPin 1

Adafruit_GPS adafruit_gps;

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

bool GPS::isFixFound() {
    return adafruit_gps.fix;
}

bool GPS::refreshNMEA() {
    adafruit_gps.read();
    if (adafruit_gps.newNMEAreceived()) {
        adafruit_gps.parse(adafruit_gps.lastNMEA());
        return true;
    }
    return false;
}
