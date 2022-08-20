#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "../headers/GPS.h"

#define RXPin 0
#define TXPin 1

SoftwareSerial GPS::ss(RXPin, TXPin);
TinyGPSPlus GPS::tinyGPS;

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
        if (tinyGPS.encode(this->ss.read())) {
            if (tinyGPS.location.isValid()) {
                return tinyGPS.location.lat();
            }
        }
    }
    return 0.0;
}

uint32_t GPS::getTime() {
    while (ss.available() > 0) {
        if (tinyGPS.encode(this->ss.read())) {
            if (tinyGPS.location.isValid()) {
                return tinyGPS.time.value();
            }
        }
    }
    return 0.0;
}

bool GPS::isGPSValid() {
    while (this->ss.available() > 0) {
        bool isEncoded = this->tinyGPS.encode(this->ss.read());
        return isEncoded && this->tinyGPS.location.isValid();
    }
    return false;
}
