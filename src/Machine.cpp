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

    void blink() {
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        digitalWrite(LED_BUILTIN, HIGH);
    }

public:

    StateMachine(int status) {
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

        groundState = GroundState();
        ascentState = AscentState();

        running = true;

        blink();
    }

    void run() {

        running = true;

        blink();

        currentState->setup();

        while(running) {
            
            blink();
            currentState->mainLoop();

        }
    }
    

};