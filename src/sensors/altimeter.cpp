#include <Wire.h>
#include <Adafruit_MPL3115A2.h>


class Altimeter {

public:
  Adafruit_MPL3115A2 altSetup() {
    Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
    if (!baro.begin()) {
      Serial.println("Couldn't find sensor");
    }
    return baro;
  }

};