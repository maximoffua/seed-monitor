//
// Created by smaximov on 20.02.24.
//

#include "main.hpp"
#include "AccessPoint.hpp"
WiFiServer server(80);

int MTech::AccessPoint::status = WL_IDLE_STATUS;

void setup() {
//Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Access Point Web Server");

    pinMode(led, OUTPUT);      // set the LED pin mode

    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
        Serial.println("Please upgrade the firmware");
    }

    MTech::AccessPoint::start();
    // wait 4 seconds for connection:
    delay(4000);

    // start the web server on port 80
    server.begin();

    // you're connected now, so print out the status
    printWiFiStatus();
}

void loop() {

}

void printWiFiStatus() {
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
