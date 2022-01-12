#include "../headers/state.hpp"
#include "../sensors/altimeter.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <Adafruit_MPL3115A2.h>

// Ground state class (before launch)

class GroundState : public State {  

private:
    Adafruit_MPL3115A2 baro;
    Altimeter alt;

public:

    void setup() {
        alt = Altimeter();
        baro = alt.altSetup();
    }

    void mainLoop() override {
        String dat = baro.getAltitude();
        //printf("%s", dat);
        Serial.begin(9600);
        // printf("The state machine is working!\n");
        // printf("You are on the ground state.\n");
        Serial.println(dat);
    }

};