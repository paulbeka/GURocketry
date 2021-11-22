#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

// Power by connecting Vin to 3-5V, GND to GND
// Uses I2C - connect SCL to the SCL pin, SDA to SDA pin
// See the Wire tutorial for pinouts for each Arduino
// http://arduino.cc/en/reference/wire

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit_MPL3115A2 test!");
  Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
  }
}

// have to run setup code in the main file and then call this function
String getData(Adafruit_MPL3115A2 baro) {
  // format: <altitude>;<temperature>;<pressure>
  // altitude in meters, temperature in deg C, pressure in Inches (Hg), so Hg\t
  Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
  String finalStr = String(String(baro.getAltitude) + ";" + String(baro.getTemperature) + ";" + String(baro.getPressure));
  return finalStr;
}


void loop() {
  
  Serial.println(getData());

 }