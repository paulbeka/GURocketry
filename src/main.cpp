#include <stdio.h>
#include <stdlib.h>

#include "Machine.cpp"


// call setup functions
int setup() {
    StateMachine mainMachine = StateMachine(0);

    mainMachine.run();
}


int main() {

    setup();

}