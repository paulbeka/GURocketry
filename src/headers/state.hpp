#ifndef STATE_HPP
#define STATE_HPP

#include <string>

class State {

public:
    virtual ~State() {};
    virtual void setup() = 0;
    virtual void mainLoop() = 0;
    virtual bool getInterrupt() = 0;
    virtual std::string getState() = 0;
};

#endif