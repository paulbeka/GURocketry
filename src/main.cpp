#include <Arduino.h>

#include "Machine.cpp"

void setup() {
  StateMachine mainMachine = StateMachine(0);

  mainMachine.run();
}

void loop() {
  // put your main code here, to run repeatedly:
}