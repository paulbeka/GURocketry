#include "../headers/state.hpp"
#include <stdlib.h>
#include <stdio.h>

// Ground state class (before launch)

class GroundState : public State {  

public:

    void setup() {

    }

    void mainLoop() override {
        printf("The state machine is working!\n");
        printf("You are on the ground state.\n");
    }

};