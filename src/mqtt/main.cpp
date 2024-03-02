//
// Created by smaximov on 20.02.24.
//

#include "wifi.hpp"

WiFiServer server(80);

using namespace MTech;

namespace MTech {
int Wireless::status = WL_IDLE_STATUS;
}

void setup() {
//Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        delay(20); // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Access Point Web Server");

    pinMode(led, OUTPUT);      // set the LED pin mode

    // check for the WiFi module:
    if (!Wireless::checkModule()) {
        Serial.println("WiFi module is not available");
        // don't continue
        while (true) { delay(20); }
    }

    AccessPoint::status = WiFi.status();
    // attempt to connect to WiFi network:
    while (AccessPoint::status != WL_CONNECTED) {
        Serial.print("Attempting initial connection to WPA SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network:
        AccessPoint::status = static_cast<wl_status_t>(WiFi.begin("BigBrother", "88498849"));
        lefteye();
        righteye();
        matrix.renderBitmap(frame, 8, 12);
        delay(1000);
        wink();
        matrix.renderBitmap(frame, 8, 12);

        // wait 15 seconds for connection:
        delay(14000);
    }

    // you're connected now, so print out the data:
    lefteye();
    righteye();
    mouth();
    matrix.renderBitmap(frame, 8, 12);
    Serial.print("You're connected to the network");
    Wireless::printCurrentNet();
    Wireless::printWifiData();

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
