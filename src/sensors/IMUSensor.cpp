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

    // Solutions?
    /* 
        Test read_all_data.ino from bno repo
        Try get all sensor data - maybe the calling of multiple events at different times ruins values
        Use of and without samplerate delay to see if clearance of buffer?
        In specific x y z aquisition functions, try save data in variable before samplerate delay then return

        ref:
        https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor
        https://github.com/adafruit/Adafruit_BNO055   
    */ 

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

        String dataString = "ax: " + String(accX) + " ay: " + String(accY) + " az: " + String(accZ) + " ox: "; 
        dataString += String(omegaX) + " oy: " + String(omegaY) + " oz: " + String(omegaZ);
        dataString += " qw: " + String(q[0]) + " qx: " + String(q[1]) + " qy: " + String(q[2]) + " qz: " + String(q[3]);

        Serial.print(dataString);
        delay(BNO055_SAMPLERATE_DELAY_MS);
        return dataString;
        
        // https://github.com/adafruit/Adafruit_BNO055/blob/master/examples/read_all_data/read_all_data.ino
    }

    void getCal() {
        // hopefully prints calibration
        uint8_t system, gyro, accel, mag = 0;
        bno.getCalibration(&system, &gyro, &accel, &mag);
        Serial.println();
        Serial.print("Calibration: Sys=");
        Serial.print(system);
        Serial.print(" Gyro=");
        Serial.print(gyro);
        Serial.print(" Accel=");
        Serial.print(accel);
        Serial.print(" Mag=");
        Serial.println(mag);
        // https://github.com/adafruit/Adafruit_BNO055/blob/master/examples/read_all_data/read_all_data.ino
    }

    void getDetails() {
        /* Create a sensor_t object in memory to filled below */
        sensor_t sensor;

        /* Get the sensor details and place them in 'sensor' */
        bno.getSensor(&sensor);

        /* Display the sensor details */
        Serial.println("------------------------------------");
        Serial.print  ("Sensor:       "); Serial.println(sensor.name);
        Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
        Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
        Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
        Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
        Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
        Serial.println("------------------------------------");
        Serial.println("");
        // https://learn.adafruit.com/using-the-adafruit-unified-sensor-driver/how-does-it-work#void-getevent-sensors-event-t-star
    }

    // Extra Joe data - 

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
};