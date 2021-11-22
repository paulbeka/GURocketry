#include "Arduino.h"
#include "GPS_parseData.h"
#include <Adafruit_GPS.h>
#include <stdexcept>

using namespace std;

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

#define GPSEcho false

uint32_t timer = millis();

// Empty constructor
GPSData::GPSData() {
}

double* GPSData::obtainData() {
	static double dataList[3];
	// Reading data fast enough, at 115200 bauds (11520 characters per second)
	Serial.begin(115200);
	Serial.println("Parsing test...");

	// Default baud rate for Adafruit GPS
	GPS.begin(9600);
	// Obtain recommended minimum and GGA (fix data) including data
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	// This will turn on only "minimum recommended"
	// Don't want because we need the fix data
	// GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1Hz update rate

	// Request updates on antenna status, comment out to keep quiet
	// Don't really need this.
	// GPS.sendCommand(PGCMD_ANTENNA);

	delay(1000);

	//Ask for firmware version
	GPSSerial.println(PMTK_Q_RELEASE);

	while (true) {
		char c = GPS.read();
		int counter = 0;
		while (!GPS.fix) {
			counter += 1;
			if (c) {
				// Need to use 'c' to avoid unused variable error
				Serial.print("");
			}
			// Delay for finding fix (0.1s / 100ms)
			delay(100);
			Serial.println("Waiting on fix...");
			if (counter == 10) {
				dataList[0] = 0;
				dataList[1] = 0;
				dataList[2] = 0;
				// Means that the fix is taking too long and we return empty list
				return dataList;
			}
		}
		Serial.println("Fix: " + GPS.fix);
		// Latitude format = DDMM.MMMM (degrees then minutes format)
		// Longitude format = DDDMM.MMMM (degrees then minutes format)

		_latitude = GPS.latitude;
		_longitude = GPS.longitude;
		_altitude = GPS.altitude;
		// Do some filtering on the _latitude and _longitude

		dataList[0] = _latitude;
		dataList[1] = _longitude;
		dataList[2] = _altitude;
		return dataList;
	}
}