#include "../headers/state.hpp"
#include <string>

// Flight state class (during flight)

class AscentState : public State {

private:
    // move to next state
    bool interrupt = false;
    std::string stateName = "Ascent";

public:
    void setup() {

    }

    void mainLoop() {

    }

    bool getInterrupt() {
        return interrupt;
    }

    std::string getState() {
        return name;
    }

};