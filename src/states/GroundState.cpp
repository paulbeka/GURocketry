#include "../headers/state.hpp"
#include "sensors/altimeter.cpp"
#include "sensors/barometer.cpp"
#include "sensors/GPS_parseData.cpp"
#include <stdlib.h>
#include <stdio.h>

// Ground state class (before launch)

class GroundState : public State {  

private:
    
    // Sensors [CREATE INCLUDE FILE FOR THESE]
    Adafruit_MPL3115A2 altSensor;
    Barometer barometer;
    GPSClass gps;

    // interrupt this process and move to the next state.
    bool interrupt = false;

public:

    GroundState(Adafruit_MPL3115A2 alt, Barometer baro, GPSClass GPS) {
        // define the sensors needed
        altSensor = alt;
        barometer = baro;
        gps = GPS;
    }

    GroundState() = default;

    void setup() {
        Serial.begin(9600);;
    }

    void mainLoop() override {
        
        // print sensor data
        Serial.println(altSensor.getAltitude());
        Serial.println(barometer.getSensorData());
        Serial.println(gps.obtainData()[0]);

    }

    bool getInterrupt() {
        return interrupt;
    }

};