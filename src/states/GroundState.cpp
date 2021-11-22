#include "../headers/state.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <Adafruit_MPL3115A2.h>
#include "sensors/altimeter.cpp"

// Ground state class (before launch)

class GroundState : public State {  

private:
    Adafruit_MPL3115A2 baro;

public:

    void setup() {
        baro = altimeterSetup();
    }

    void mainLoop() override {
        //String dat = getAltimeterData(baro);
        //printf("%s", dat);
        printf("The state machine is working!\n");
        printf("You are on the ground state.\n");
    }

};