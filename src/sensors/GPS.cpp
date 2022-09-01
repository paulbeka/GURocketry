// #include <Arduino.h>
// #include <TinyGPS++.h>
// #include <SoftwareSerial.h>
// #include "../headers/GPS.h"

// #define RX_PIN 15
// #define TX_PIN 14

// SoftwareSerial ss(RX_PIN, TX_PIN);
// TinyGPSPlus tinyGPS;

// GPS::GPS() { 
// }

// void GPS::setup() {
//     Serial.println("Initialising GPS client...");
//     ss.begin(9600);
//     if (!ss) {
//         Serial.println("Error initialising Software Serial!");
//     }
//     Serial.println("GPS client initialised.");
// }

// String GPS::getAllData() {

//     while (ss.available() > 0) {
//         if (tinyGPS.encode(ss.read())) {
//             if (tinyGPS.location.isValid()) {

//                 uint32_t tim = tinyGPS.time.value();
//                 double lat = tinyGPS.location.lat();
//                 double lng = tinyGPS.location.lng();

//                 Serial.println(tinyGPS.location.lat());
//                 Serial.println(tinyGPS.location.lng());

//                 //String dataString = " | GPS - Time: " + String(tim) + " Lat: " + String(lat) + " Lng: " + String(lng); 

//                 String dataString = "done";
//                 return dataString;
//             }
//         }
//     }
// }

// double GPS::getLng() {

//     while (ss.available() > 0) {
//         delay(500);
//         if (tinyGPS.encode(ss.read())) {
//             if (tinyGPS.location.isUpdated()) {
//                 return tinyGPS.location.lng();
//             }
//         }
//     }
//     //return 11;
// }

// double GPS::getLat() {
    
//     while (ss.available() > 0) {
//         delay(500);
//         if (tinyGPS.encode(ss.read())) {
//             if (tinyGPS.location.isUpdated()) {
//                 return tinyGPS.location.lat();
//             }
//         }
//         // else if (!(tinyGPS.encode(ss.read()))) {
//         //     return 404;
//         // }
//     }
//     //return 11;
// }

// uint32_t GPS::getTime() {
//     while (ss.available() > 0) {
//         if (tinyGPS.encode(ss.read())) {
//             if (tinyGPS.location.isValid()) {
//                 return tinyGPS.time.value();
//             }
//         }
//     }
//     //return 0.0;
// }

// // Extra Joe data - 

// double GPS::getSpeed() {
//     while (ss.available() > 0) {
//         if (tinyGPS.encode(ss.read())) {
//             if (tinyGPS.speed.isValid()) {
//                 return tinyGPS.speed.mps();
//             }
//         }
//     }
//     //return 0.0;
// }

// double GPS::getAlt() {
//     while (ss.available() > 0) {
//         if (tinyGPS.encode(ss.read())) {
//             if (tinyGPS.altitude.isValid()) {
//                 return tinyGPS.altitude.meters();
//             }
//         }
//     }
//     //return 0.0;
// }

// double GPS::getDistance() {
//     static const double lpLat = 55.7265907, lpLng = -4.8115762;
    
//     while (ss.available() > 0) {
//         if (tinyGPS.encode(ss.read())) {
//             if (tinyGPS.location.isValid()) {
//                 return TinyGPSPlus::distanceBetween(tinyGPS.location.lat(), tinyGPS.location.lng(), lpLat, lpLng);
//             }
//         }
//     }
// }

// int GPS::getSatNo() {
//     while (ss.available() > 0) {
//         if (tinyGPS.encode(ss.read())) {
//             return tinyGPS.satellites.value();
//         }
//     }
// }

