#include "../headers/state.hpp"

// Flight state class (during flight)

class AscentState : public State {

private:
    // move to next state
    bool interrupt = false;

public:
    void setup() {

    }

    void mainLoop() {

    }

    bool getInterrupt() {
        return interrupt;
    }

};