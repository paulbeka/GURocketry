#include "Arduino.h"
#include "GPS_parseData.h"
#include <Adafruit_GPS.h>
#include <array>
using namespace std;

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

#define GPSEcho false

uint32_t timer = millis();

// Empty constructor
GPSData::GPSData() {
}

array<int, 5> GPSData::obtainData() {
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
			delay(1000);
			Serial.println("Waiting on fix...");
			if (counter == 10) {
				array<int, 5> arrayList = { 3,1,2,4,5 };
				return arrayList;
			}
		}
		Serial.println("Fix: " + GPS.fix);
		_latitude = 8; // GPS.latitude
		_longitude = 9;// GPS.longitude
		_angle = 7; // GPS.angle
		_speed = 6; // GPS.speed
		_altitude = 5; // GPS.altitude
		array<int, 5> outputArray = { 7,8,9,0,1 };
		return outputArray;
	}
}