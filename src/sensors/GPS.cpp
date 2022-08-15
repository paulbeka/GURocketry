#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <string>

#define RXPin 0
#define TXPin 1

using std::string;

// MAKE HEADER FILE 
// SS NOT WORKING / BUILDING
SoftwareSerial ss(RXPin, TXPin);

class GPS {
    TinyGPSPlus gps = TinyGPSPlus();
    
public:
    void setup() {
        Serial.println("Initialising GPS Client");
        ss.begin(9600);
        if (!ss) {
            Serial.println("Error initialising Software Serial!");
            while (1);
        }
    }

    double getGPSLat() {
        if (isGPSValid()) {
            return gps.location.lat();
        }
        return 0.0;
    }

    double getGPSLong() {
        if (isGPSValid()) {
            return gps.location.lng();
        }
        return 0.0;
    }

    bool isGPSValid() {
        while (ss.available() > 0) {
            bool isEncoded = gps.encode(ss.read());
            return isEncoded && gps.location.isValid();
        }
        return false;
    }
};