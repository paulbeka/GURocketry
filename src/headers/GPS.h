#ifndef GPS_H_
#define GPS_H_

#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

class GPS {
    private:
        bool refreshNMEA();
    public:
        Adafruit_GPS adafruit_gps;
        GPS();
        void setup();
        float getLat();
        float getLong();
        String getTime();
        bool isFixFound();
};

#endif 