#pragma once
/*
	GPS_parseData.h - GURocketry library
	Last update: 10/11/21 - 18:21 - Sam Jackson
*/

#ifndef GPS_parseData_h
#define GPS_parseData_h
#include <array>

using namespace std;

#include "Arduino.h"

class GPSData {
	public:
		GPSData();
		double* obtainData();
	private:
		int _id;
		int _latitude;
		int _longitude;
		int _angle;
		int _speed;
		int _altitude;
};

#endif