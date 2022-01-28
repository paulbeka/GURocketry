#include <stdlib.h>
#include <stdio.h>
#include <Adafruit_MPL3115A2.h>

#include "./headers/machine.hpp"
#include "states/Ascent.cpp"
#include "states/GroundState.cpp"


// This class handles the different states of the computer

class StateMachine : public Machine {

private:
    GroundState groundState;
    AscentState ascentState;
    State* currentState;

    Adafruit_MPL3115A2 altSensor;
    Altimeter alt;
    Barometer barometer;
    GPSClass GPSSensor;

public:

    StateMachine(int status) {
        
        alt = Altimeter();
        altSensor = alt.altSetup();
        barometer = Barometer();
        GPSSensor = GPSClass();
        
        setup();

        currentState = choseState(status);
    }

    State* choseState(int status) {
        switch(status) {
            case 0: return &groundState;
            case 1: return &ascentState;
        }
        return NULL;
    }
    
    void setup() {
        // Adafruit_MPL3115A2 baro = altSetup();

        barometer.setup();

        groundState = GroundState(altSensor, barometer, GPSSensor);
        ascentState = AscentState();

        running = true;

    }

    void run() {

        running = true;

        currentState->setup();

        while(running && !currentState->getInterrupt()) {
            
            currentState->mainLoop();

        }
    }
    

};