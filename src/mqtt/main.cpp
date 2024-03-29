//
// Created by smaximov on 20.02.24.
//

#include <Arduino.h>
#include <wifi/WiFiClient.hpp>
#include "r4/config.hpp"
#include "r4/LedMatrix.hpp"
#include "wifi/Connections.hpp"
#include "r4/data.hpp"
#include "r4/sensor.hpp"

void processWlan();
void processClients();
void processClient(WiFiClient &client);
void processStreams();
void readAll(WiFiClient &client);
void sendData(Print &stream);

using Wireless = mtech::WiFiClient;
using STM = Wireless::StateMachine;

mtech::WiFiClient::StateMachine::action mtech::WiFiClient::StateMachine::stateMap[State::count] = {};


Wireless wlan{};
WiFiServer server(80);

mtech::Connections<5> clients{};
mtech::Connections<3> streams{};

Sensor sensors[] = {Sensor{A0}, Sensor{A1}, Sensor{A2}, Sensor{A3}};

//mtech::LEDMatrix matrix{};
//ArduinoLEDMatrix matrix{};

void setup()
{
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        delay(20); // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Initializing...");

    pinMode(led, OUTPUT);

    // init ADC pins
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);

    Serial.println("Initializing matrix...");
//    matrix.begin();

    // check for the WiFi module:
    if (!Wireless::checkModule()) {
        Serial.println("WiFi module is not available");
        // don't continue
        while (true) { delay(20); }
    }

    STM::stateMap[Wireless::State::Idle] = [](Wireless *w)
    {
        Serial.print("Connecting to WLAN: ");
        Serial.println(SSID);
        Serial.print("          Password: ");
        Serial.println(PASSWORD);
        auto res = w->connect(SSID, PASSWORD);
        Serial.print("Wireless is idle, status: ");
        Serial.println(mtech::statusToString(res));
    };
    STM::stateMap[Wireless::State::Connecting] = [](Wireless *w)
    {
        if (w->elapsed() % 1000 < 950) {
            return;
        }
//        matrix.frown();
        auto res = w->updateStatus();
        Serial.print("Connecting to WiFi... ");
        Serial.println(mtech::statusToString(res));
//        STM::stateMap[Wireless::State::Connecting] = nullptr;
    };
    STM::stateMap[Wireless::State::Connected] = [](Wireless *w)
    {
        auto client = server.available();
        if (!client || clients.capacity() == clients.size()) {
            return;
        }
        for (const auto &existing: clients) {
            if (client == existing) {
                return;
            }
        }
        for (const auto &existing: streams) {
            if (client == existing) {
                return;
            }
        }
        clients.push(client);
        if (client.connected()) {
            Serial.println("New client connected");
            Serial.print("Client IP: ");
            Serial.println(client.remoteIP());
        }
    };

    Serial.println("WiFi module is started");
}

void loop()
{
    const auto start = millis();
    processWlan();
//    matrix.loop();
    processClients();
    processStreams();
    const auto end = millis();
    Serial.print("Loop duration: ");
    Serial.println(end - start);
}

void processWlan()
{
    auto state = wlan.getState();
    if (auto ns = wlan.loop(); ns != state) {
        switch (ns) {
            case Wireless::State::Connected:
//                matrix.face();
                Serial.println("Connected to WiFi");
                Wireless::printWifiData();
                server.begin();
                break;
            case Wireless::State::Disconnected:
//                matrix.frown();
                Serial.println("Disconnected from WiFi");
                break;
        }
    }
}

void processClients()
{
    for (auto &client : clients.clients) {
        if (!client) continue;
        if (!client.connected()) {
            Serial.println("Client disconnected");
            client.stop();
            continue;
        }
        processClient(client);
    }
}

void processClient(WiFiClient &client)
{
    static int route {0};
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.available()) {             // if there's bytes to read from the client,
        char c = static_cast<char>(client.read()); // read a byte, then
        Serial.write(c);                    // print it out to the serial monitor
        if (c == '\n' && route == 0) {                    // if the byte is a newline character
            route = 1;
        } else if (c != '\r') {    // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
        }
        // Check if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /adc")) {
            route = 2;
        } else if (currentLine.endsWith("GET /")) {
            route = 1;
        }
    }

    // Streaming route /adc
    if (route == 2) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/event-stream");
        client.println("Connection: keep-alive");
        client.println("Cache-Control: no-cache");
        client.println();
        client.println("event: connect");
        client.println("data: starting stream");
        streams.push(client);
        clients.disable(client);
    } else {
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
            Serial.print("Sending HTML page to client... ");
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // the content of the HTTP response follows the header:
            client.print(indexHtml);
            // The HTTP response ends with another blank line:
            client.println();
            delay(8);
            client.stop();
            Serial.println("OK");
            Serial.flush();
        } else {      // if you got a newline, then clear currentLine:
            currentLine = "";
        }
    }
    route = 0;
}

std::tuple<int, int, int, int> measure()
{
    int adc0 = analogRead(A0);
    int adc1 = analogRead(A1);
    int adc2 = analogRead(A2);
    int adc3 = analogRead(A3);
    return {adc0, adc1, adc2, adc3};
}
void processStreams()
{
    static uint32_t last = millis();
    const auto& [adc0, adc1, adc2, adc3] = measure();
    if (millis() - last > 3000) {
        Serial.print("measuring ADC... ");
        Serial.print(adc0);
        Serial.print(" | ");
        Serial.print(adc1);
        Serial.print(" | ");
        Serial.print(adc2);
        Serial.print(" | ");
        Serial.print(adc3);
        Serial.print(" / ");
        Serial.print(streams.size());
        Serial.println(" streams");
        last = millis();
    }
    for (auto &stream: streams.clients) {
        if (!stream) {
            continue;
        }
        readAll(stream);
        if (!stream.connected()) {
            Serial.println("disconnected");
            stream.stop();
            continue;
        }
        stream.print("data: [");
        stream.print(adc0);
        stream.print(',');
        stream.print(adc1);
        stream.print(',');
        stream.print(adc2);
        stream.print(',');
        stream.print(adc3);
        stream.println(']');
        stream.println();
        stream.flush();
    }
}
void readAll(WiFiClient &client)
{
    while (client.available()) {
        char c = static_cast<char>(client.read());
        Serial.write(c);
    }
}
void sendData(Print &stream)
{
    stream.print("data: [");
    for (uint8_t i = 0; i < (sizeof(sensors) / sizeof(Sensor)); ++i) {
        stream.print("ADC");
        stream.print(i);
        stream.print(':');
        auto& sens = sensors[i];
        stream.print(sens.value());
        if (i < (sizeof(sensors) / sizeof(Sensor)) - 1) {
            stream.print('\t');
        }
    }
}
