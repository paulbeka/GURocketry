#ifndef STATE_HPP
#define STATE_HPP

class State {

public:
    virtual void setup() = 0;

    virtual void mainLoop() = 0;
};

#endif