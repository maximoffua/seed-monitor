//
// Created by smaximov on 20.02.24.
//

#ifndef SEEDER_ACCESSPOINT_HPP
#define SEEDER_ACCESSPOINT_HPP
#include "Arduino.h"
#include "WiFiS3.h"
#include "config.hpp"

namespace MTech {

class AccessPoint {
public:
    static int status;
    static const char* firmwareVersion() {
        return WiFi.firmwareVersion();
    }
    static void printWiFiStatus() {
        // print the SSID of the network you're attached to:
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        // print your WiFi shield's IP address:
        IPAddress ip = WiFi.localIP();
        Serial.print("IP Address: ");
        Serial.println(ip);

        // print where to go in a browser:
        Serial.print("To see this page in action, open a browser to http://");
        Serial.println(ip);
    }

    static void start() {
        WiFi.config(IPAddress(192,168,12,1));
        WiFi.setHostname(HOSTNAME);

        // print the network name (SSID);
        Serial.print("Creating access point named: ");
        Serial.println(ssid);

        // Create open network. Change this line if you want to create an WEP network:
        status = WiFi.beginAP(ssid, pass);
        if (status != WL_AP_LISTENING) {
            Serial.println("Creating access point failed");
            // don't continue
            while (true);
        }
    }
};

} // MTech

#endif //SEEDER_ACCESSPOINT_HPP
