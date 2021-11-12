#ifndef MACHINE_H
#define MACHINE_H
#define STATES_COUNT 4  // number of rocket states

#include "state.hpp"

class Machine {

protected:
     State groundState;
     State flightState;
     State currentState;

     bool running;

public:

    virtual void setupStates() {};

    virtual void choseState() {};

};

#endif