#ifndef GPS_H_
#define GPS_H_

#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class GPS {
    public:
        GPS();
        void setup();
        double getLat();
        double getLng();
        uint32_t getTime();
        double getSpeed();
        double getAlt();
};

#endif 