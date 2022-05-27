#include "../headers/state.hpp"
#include <string>

// Reinitialise class

class Restart : public State {

private:
    std::string stateName = "Restart";

public:
    void setup() {

    }


    void mainLoop() {

    }

    std::string getState() {
        return stateName;
    }

};