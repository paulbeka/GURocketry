#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)


class IMUSensor {

    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

public:
    void setup() {

        /* Initialise the sensor */
        if(!bno.begin()) {
            /* There was a problem detecting the BNO055 ... check your connections */
            Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
            while(1);
        }

        delay(1000);

        bno.setExtCrystalUse(true);
    }

    String getSensorData() {
        sensors_event_t event;
        bno.getEvent(&event);

        String finalStr = String(String(event.orientation.x) + ";" + String(event.orientation.y) + ";" + String(event.orientation.z));
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return finalStr;
    }

    float getAcceleration() {
        sensors_event_t event;
        bno.getEvent(&event);
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return event.acceleration.y;
    }
};