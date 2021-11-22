#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

Adafruit_MPL3115A2 altimeterSetup() {
  Serial.begin(9600);
  Serial.println("Adafruit_MPL3115A2 test!");
  Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
  if (! baro.begin()) {
    Serial.println("Couldn't find sensor");
    return;
  }
  return baro;
}
