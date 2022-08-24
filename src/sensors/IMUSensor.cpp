#include <Wire.h>
#include <array>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

using std::array;

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

    // Extra Joe data - 
    // maybe functions return vectors/arrays instead of each indiviudal component?
    // event.gyro or event.orientation for obtaining omega_

    float getAccX() {
        sensors_event_t event;
        bno.getEvent(&event);
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return event.acceleration.x;
    }

    float getAccY() {
        sensors_event_t event;
        bno.getEvent(&event);
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return event.acceleration.y;
    }

    float getAccZ() {
        sensors_event_t event;
        bno.getEvent(&event);
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return event.acceleration.z;
    }

    float getOmegaX() {
        sensors_event_t event;
        bno.getEvent(&event);
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return event.gyro.x;
    }

    float getOmegaY() {
        sensors_event_t event;
        bno.getEvent(&event);
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return event.gyro.y;
    }

    float getOmegaZ() {
        sensors_event_t event;
        bno.getEvent(&event);
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return event.gyro.z;
    }

    std::array<double, 4> getQuat() {
        imu::Quaternion quat = bno.getQuat();
        std::array<double, 4> q {quat.w(), quat.x(), quat.y(), quat.z()};      
        return q;
    }

    // write quaternion function: 
    // reference: https://github.com/adafruit/Adafruit_BNO055/blob/master/utility/quaternion.h
    // example: https://github.com/adafruit/Adafruit_BNO055/blob/master/examples/webserial_3d/webserial_3d.ino
};