#include "./headers/state.hpp"
#include <stdlib.h>
#include <stdio.h>

// Ground state class (before launch)

class GroundState : public State {  

public:

    int test = 0;

    void setup() override {

    }


    void mainLoop() override {
        printf("The state machine is working!\n");
        printf("You are on the ground state.\n");
    }

};