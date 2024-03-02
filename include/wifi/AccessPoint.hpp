//
// Created by smaximov on 20.02.24.
//

#ifndef SEEDER_ACCESSPOINT_HPP
#define SEEDER_ACCESSPOINT_HPP

#include "config.hpp"
#include "WiFiS3.h"

namespace MTech {

class Wireless {
public:
    static int status;

    static bool checkModule() {
        // check for the presence of the shield:
        if (WiFi.status() == WL_NO_MODULE) {
            return false;
        }
        String fv = CWifi::firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
            Serial.println("Please upgrade the firmware");
        }
        return true;
    }

    static unsigned char
    connect(const char *ssid, const char *pass, int timeout = 15'000) {
        // attempt to connect to WiFi network:
        auto start = millis();
        // Connect to WPA/WPA2 network:
        status = static_cast<int>(WiFi.begin(ssid, pass));
        Serial.print("Attempting initial connection to WPA SSID: ");
        Serial.println(ssid);
        while (status != WL_CONNECTED && (millis() - start) < timeout) {
            Serial.print(".");
            delay(1000);
            status = WiFi.status();
        }
        return WiFi.status();
    }

    [[maybe_unused]] static void printWiFiStatus() {
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

    static void printMacAddress(byte mac[]) {
        for (int i = 5; i >= 0; i--) {
            if (mac[i] < 16) {
                Serial.print("0");
            }
            Serial.print(mac[i], HEX);
            if (i > 0) {
                Serial.print(":");
            }
        }
        Serial.println();
    }

    static void printWifiData() {
        // print your board's IP address:
        IPAddress ip = WiFi.localIP();
        Serial.print("IP Address: ");

        Serial.println(ip);

        // print your MAC address:
        byte mac[WL_MAC_ADDR_LENGTH];
        WiFi.macAddress(mac);
        Serial.print("MAC address: ");
        printMacAddress(mac);
    }

    static void printCurrentNet() {
        // print the SSID of the network you're attached to:
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        // print the MAC address of the router you're attached to:
        byte bssid[6];
        WiFi.BSSID(bssid);
        Serial.print("BSSID: ");
        printMacAddress(bssid);

        // print the received signal strength:
        long rssi = WiFi.RSSI();
        Serial.print("signal strength (RSSI):");
        Serial.println(rssi);

        // print the encryption type:
        byte encryption = WiFi.encryptionType();
        Serial.print("Encryption Type:");
        Serial.println(encryption, HEX);
        Serial.println();
    }
};

class AccessPoint : public Wireless {
public:
    static void start(const char *ssid = "maxPilot", const char *pass = "agromaranta") {
        WiFi.config(IPAddress(192, 168, 12, 1));
        WiFi.setHostname(HOSTNAME);

        // print the network name (SSID);
        Serial.print("Creating access point named: ");
        Serial.println(ssid);

        // Create open network. Change this line if you want to create an WEP network:
        while (status != WL_AP_LISTENING) {
            status = WiFi.beginAP(ssid, pass);
            delay(2000);
        }
    }
};

} // MTech

#endif //SEEDER_ACCESSPOINT_HPP
