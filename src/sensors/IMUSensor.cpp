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
        Serial.println("Initialising IMU sensor...");

        if(!bno.begin()) {
            /* There was a problem detecting the BNO055 ... check your connections */
            Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
            while(1);
        }
        
        delay(1000);
        bno.setExtCrystalUse(true);
        Serial.println("IMU sensor initialised.");
    }

    void getDetails() {
        /* Create a sensor_t object in memory to filled below */
        sensor_t sensor;

        /* Get the sensor details and place them in 'sensor' */
        bno.getSensor(&sensor);

        /* Display the sensor details */
        Serial.println("");
        Serial.println("------------------IMU Sensor Details------------------");
        Serial.print  ("Sensor:       "); Serial.println(sensor.name);
        Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
        Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
        Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
        Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
        Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
        Serial.println("-----------------------------------------------------");
        Serial.println("");
        // https://learn.adafruit.com/using-the-adafruit-unified-sensor-driver/how-does-it-work#void-getevent-sensors-event-t-star
    }

    void getCalVals() {
        /* Prints calibration values of sensor to Serial */
        uint8_t system, gyro, accel, mag = 0;
        bno.getCalibration(&system, &gyro, &accel, &mag);
        Serial.println("Calibration: Sys=");
        Serial.print(system);
        Serial.print(" Gyro=");
        Serial.print(gyro);
        Serial.print(" Accel=");
        Serial.print(accel);
        Serial.print(" Mag=");
        Serial.println(mag);
        // https://github.com/adafruit/Adafruit_BNO055/blob/master/examples/read_all_data/read_all_data.ino
    }

    String getAllSensorData() {
        // seperate events specifying vector type for readings, all in one function
        sensors_event_t accData, angVelData;
        bno.getEvent(&accData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        bno.getEvent(&angVelData, Adafruit_BNO055::VECTOR_GYROSCOPE);
        imu::Quaternion quat = bno.getQuat();

        float accX = accData.acceleration.x;
        float accY = accData.acceleration.y;
        float accZ = accData.acceleration.z;

        float omegaX = angVelData.gyro.x;
        float omegaY = angVelData.gyro.y;
        float omegaZ = angVelData.gyro.z;

        std::array<double, 4> q {quat.w(), quat.x(), quat.y(), quat.z()};      

        String dataString = "accX: " + String(accX) + " accY: " + String(accY) + " accZ: " + String(accZ); 
        dataString += " omegaX: " + String(omegaX) + " omegaY: " + String(omegaY) + " omegaZ: " + String(omegaZ);
        dataString += " quatW: " + String(q[0]) + " quatX: " + String(q[1]) + " quatY: " + String(q[2]) + " quatZ: " + String(q[3]);

        // delay(BNO055_SAMPLERATE_DELAY_MS);
        return dataString;
        // https://github.com/adafruit/Adafruit_BNO055/blob/master/examples/read_all_data/read_all_data.ino
    }

    // unsure if delay messes with sensor reading - perhaps by clearing buffer - so have stored in variable to fix

    float getAccX() {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        float x = event.acceleration.x;
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return x;
    }

    float getAccY() {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        float y = event.acceleration.y;
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return y;
    }

    float getAccZ() {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        float z = event.acceleration.z;
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return z;
    }

    float getOmegaX() {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_GYROSCOPE);
        float x = event.gyro.x;
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return x;
    }

    float getOmegaY() {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_GYROSCOPE);
        float y = event.gyro.y;
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return y;
    }

    float getOmegaZ() {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_GYROSCOPE);
        float z = event.gyro.z;
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return z;
    }

    std::array<double, 4> getQuat() {
        imu::Quaternion quat = bno.getQuat();
        std::array<double, 4> q {quat.w(), quat.x(), quat.y(), quat.z()};      
        return q;
    }

    /* ref:
        https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor
        https://github.com/adafruit/Adafruit_BNO055
    */ 
};