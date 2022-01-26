#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address

class Barometer {

    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

public:
    void displaySensorStatus(void)
    {
    /* Get the system status values (mostly for debugging purposes) */
    uint8_t system_status, self_test_results, system_error;
    system_status = self_test_results = system_error = 0;
    bno.getSystemStatus(&system_status, &self_test_results, &system_error);

    /* Display the results in the Serial Monitor */
    Serial.println("");
    Serial.print("System Status: 0x");
    Serial.println(system_status, HEX);
    Serial.print("Self Test:     0x");
    Serial.println(self_test_results, HEX);
    Serial.print("System Error:  0x");
    Serial.println(system_error, HEX);
    Serial.println("");
    delay(500);
    }

    /**************************************************************************/
    /*
        Display sensor calibration status
    */
    /**************************************************************************/
    void displayCalStatus(void)
    {
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    /* The data should be ignored until the system calibration is > 0 */
    Serial.print("\t");
    if (!system)
    {
        Serial.print("! ");
    }

    /* Display the individual values */
    Serial.print("Sys:");
    Serial.print(system, DEC);
    Serial.print(" G:");
    Serial.print(gyro, DEC);
    Serial.print(" A:");
    Serial.print(accel, DEC);
    Serial.print(" M:");
    Serial.print(mag, DEC);
    }

    /**************************************************************************/
    /*
        Arduino setup function (automatically called at startup)
    */
    /**************************************************************************/
    void setup(void)
    {
    Serial.begin(115200);
    Serial.println("Orientation Sensor Test"); Serial.println("");

    /* Initialise the sensor */
    if(!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }

    delay(1000);

    /* Optional: Display current status */
    displaySensorStatus();

    bno.setExtCrystalUse(true);
    }

    String getSensorData() {
    sensors_event_t event;
    bno.getEvent(&event);

    String finalStr = String(String(event.orientation.x) + ";" + String(event.orientation.y) + ";" + String(event.orientation.z));
    delay(BNO055_SAMPLERATE_DELAY_MS);
    return finalStr;
    }

    void loop(void)
    {
    Serial.print(getSensorData()); 
    }
};