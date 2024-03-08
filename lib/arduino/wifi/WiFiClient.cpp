//
// Created by Sergii Maksymov on 07.03.24.
// Copyright (c) 2024 Maximoff Tech. All rights reserved.
//

#include "WiFiClient.hpp"

namespace mtech
{

unsigned char WiFiClient::connect(const char *ssid, const char *pass, int timeout)
{
    // attempt to connect to WiFi network:
    opStart = millis();
    // Connect to WPA/WPA2 network:
    status = static_cast<int>(WiFi.begin(ssid, pass));
    stm.state = StateMachine::Connecting;
//        Serial.print("Attempting initial connection to WPA SSID: ");
//        Serial.println(ssid);
//        while (status != WL_CONNECTED && (millis() - start) < timeout) {
//            Serial.print(".");
//            delay(1000);
//            status = WiFi.status();
//        }
    return WiFi.status();
}
void WiFiClient::printWiFiStatus()
{
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
void WiFiClient::printMacAddress(byte *mac)
{
    for (int i = 5; i >= 0; i--) {
        if (mac[i] < 16) {
            Serial.print("0");
        }
        Serial.print(mac[i], HEX);
        if (i > 0) {
            Serial.print(":");
        }
    }
}
void WiFiClient::printWifiData()
{
    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");

    Serial.println(ip);

    // print your MAC address:
    byte mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    printMacAddress(mac);
    Serial.println();
}
void WiFiClient::printCurrentNet()
{
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
WiFiClient::State WiFiClient::loop() noexcept
{
    static size_t ts;
    const auto& callback = StateMachine::stateMap[stm.state];
    if (callback) {
        callback(this);
    }

    if (millis() - ts > 2000) {
        ts = millis();
        updateStatus();
    }

    switch (stm.state) {
        case StateMachine::Idle:
            break;
        case StateMachine::Connecting:
            break;
        case StateMachine::Connected:
            break;
        case StateMachine::Disconnected:
            break;
    }
    return stm.state;
}
void WiFiClient::updateStatus()
{
    status = WiFi.status();
    switch (status) {
        case WL_CONNECTED:
            Serial.println("Connected");
            stm.state = StateMachine::Connected;
            break;
        case WL_CONNECT_FAILED:
            Serial.println("Connection failed");
            stm.state = StateMachine::Disconnected;
            break;
        case WL_CONNECTION_LOST:
            Serial.println("Connection lost");
            stm.state = StateMachine::Disconnected;
            break;
        case WL_NO_SSID_AVAIL:
            Serial.println("No SSID available");
            stm.state = StateMachine::Disconnected;
            break;
        case WL_DISCONNECTED:
            Serial.println("Disconnected");
            stm.state = StateMachine::Disconnected;
            break;
        default:
            stm.state = StateMachine::Idle;
    }
}
size_t WiFiClient::elapsed() const
{
    return millis() - opStart;
}
void WiFiClient::begin()
{
    status = WiFi.status();
}
bool WiFiClient::checkModule()
{
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
WiFiClient::State WiFiClient::getState() const
{
    return stm.state;
}
} // mtech