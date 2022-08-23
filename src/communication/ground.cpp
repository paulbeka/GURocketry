#include <LoRa.h>

#include <string>

#define LoRaFreq 433E6
#define RFM95_CS 6
#define RFM95_RST 22
#define RFM95_INT 23

class Ground {

public:
    void setup() {
        Serial.println("Initialising LoRa...");
        LoRa.setPins(RFM95_CS, RFM95_RST, RFM95_INT);
        if (!LoRa.begin(LoRaFreq)) {
            Serial.println("LoRa init failed!");
            while (1);
        }
        Serial.println("LoRa init successful.\n");
        LoRa.setTxPower(23); // increase power and therefore range by +20db (5-23 available)
    }

    String receiveData() {
        int packetSize = LoRa.parsePacket();

        String message = "";

        if (packetSize) {
            while (LoRa.available()) {
                message += (char)LoRa.read();
            }
            Serial.print(" | RSSI: ");
            Serial.print(LoRa.packetRssi());
            Serial.print("\n");
        }
        return message;
    }
};