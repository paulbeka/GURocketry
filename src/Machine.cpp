#include <stdlib.h>
#include <stdio.h>
#include "./headers/machine.hpp"

#include "states/Ascent.cpp"
#include "states/GroundState.cpp"

// This class handles the different states of the computer

class StateMachine : public Machine {

private:
    GroundState groundState;
    AscentState ascentState;
    State* currentState;

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
        groundState = GroundState();
        ascentState = AscentState();

        running = true;
    }

    void run() {

        running = true;

        //currentState->setup();

        while(running) {
            currentState->mainLoop();
            running = false;
        }
    }
    

};