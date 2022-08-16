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
    this->ss.begin(9600);
    if (!this->ss) {
        Serial.println("Error initialising Software Serial!");
        while (1);
    }
}

double GPS::getGPSLat() {
    if (isGPSValid()) {
        return this->tinyGPS.location.lat();
    }
    return 0.0;
}

double GPS::getGPSLong() {
    if (isGPSValid()) {
        return this->tinyGPS.location.lng();
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
