#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "../headers/GPS.h"

#define RX_PIN 0
#define TX_PIN 1

SoftwareSerial ss(RX_PIN, TX_PIN);
TinyGPSPlus tinyGPS;

GPS::GPS() { 
}

void GPS::setup() {
    Serial.println("Initialising GPS Client");
    ss.begin(9600);
    if (!ss) {
        Serial.println("Error initialising Software Serial!");
    }
}

double GPS::getLong() {
    while (ss.available() > 0) {
        if (tinyGPS.encode(ss.read())) {
            if (tinyGPS.location.isValid()) {
                return tinyGPS.location.lng();
            }
        }
    }
    return 0.0;
}

double GPS::getLat() {
    while (ss.available() > 0) {
        if (tinyGPS.encode(ss.read())) {
            if (tinyGPS.location.isValid()) {
                return tinyGPS.location.lat();
            }
        }
    }
    return 0.0;
}

uint32_t GPS::getTime() {
    while (ss.available() > 0) {
        if (tinyGPS.encode(ss.read())) {
            if (tinyGPS.location.isValid()) {
                return tinyGPS.time.value();
            }
        }
    }
    return 0.0;
}

