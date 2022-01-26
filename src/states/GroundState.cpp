#include "../headers/state.hpp"
#include "../sensors/altimeter.cpp"
#include <stdlib.h>
#include <stdio.h>

// Ground state class (before launch)

class GroundState : public State {  

private:
    Adafruit_MPL3115A2 altSensor;

public:

    GroundState(Adafruit_MPL3115A2 alt) {
        altSensor = alt;
    }

    GroundState() = default;

    void setup() {

    }

    void mainLoop() override {
        String dat = altSensor.getAltitude();
        //printf("%s", dat);
        Serial.begin(9600);
        // printf("The state machine is working!\n");
        // printf("You are on the ground state.\n");
        Serial.println(dat);
    }

};