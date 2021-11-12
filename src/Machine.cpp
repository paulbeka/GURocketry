#include <stdlib.h>
#include <stdio.h>
#include "./headers/machine.hpp"

#include "FlightState.cpp"
#include "GroundState.cpp"

// This class handles the different states of the computer

class StateMachine : public Machine {

private:
    GroundState groundState;
    FlightState flightState;
    State* currentState;

public:

    StateMachine(int status) {
        setup();

        choseState(status);   
    }

    void choseState(int status) {
        switch(status) {
            case 0: currentState = &groundState;
            case 1: currentState = &flightState;
        }
    }
    
    void setup() {
        groundState = GroundState();
        flightState = FlightState();

        running = true;
    }

    void run() {

        running = true;

        while(running) {
            currentState->mainLoop();
            running = false;
        }
    }
    

};