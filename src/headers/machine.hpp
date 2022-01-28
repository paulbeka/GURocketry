#ifndef MACHINE_H
#define MACHINE_H

class Machine {

protected:
     bool running;

public:

    virtual void setupStates() {};

    virtual void choseState() {};
    
};

#endif