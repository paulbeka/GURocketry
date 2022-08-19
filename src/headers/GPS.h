#ifndef GPS_H_
#define GPS_H_

#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class GPS {
    private:
        static SoftwareSerial ss;
        static TinyGPSPlus tinyGPS;
    public:
        GPS();
        void setup();
        double getLat();
        double getLong();
        uint32_t getTime();
        bool isGPSValid();
};

#endif 