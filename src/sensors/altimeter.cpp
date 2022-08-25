#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

class AltimeterSensor {
public:
  Adafruit_MPL3115A2 baro;

  void setup() {
    Serial.println("Initialising altimeter...");

    if (!baro.begin()) {
      Serial.println("Could not find sensor. Check wiring!");
      while(1);
    }

    // use to set sea level pressure for current location
    // this is needed for accurate altitude measurement
    // STD SLP = 1013.26 hPa
    baro.setSeaPressure(1013.26);
    Serial.println("Altimeter initialised.");
  }

  float getAltitude() {
    return baro.getAltitude();
  }

};