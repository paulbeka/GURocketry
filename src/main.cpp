#include <Arduino.h>

#include "Machine.cpp"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  StateMachine mainMachine = StateMachine(0);

  Serial.begin(9600);
  Serial.println("Hello world!");

  digitalWrite(LED_BUILTIN, HIGH);

  mainMachine.run();
}

void loop() {
  

}