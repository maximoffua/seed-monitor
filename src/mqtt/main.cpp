//
// Created by smaximov on 20.02.24.
//

#include <Arduino.h>
#include <wifi/WiFiClient.hpp>
#include "r4/config.hpp"
#include "r4/LedMatrix.hpp"
#include "wifi/Connections.hpp"
#include "r4/data.hpp"

void processWlan();
void processClients();
void processClient(WiFiClient &client);
void processStreams();



using Wireless = mtech::WiFiClient;

mtech::WiFiClient::StateMachine::action mtech::WiFiClient::StateMachine::stateMap[State::count] = {};

using STM = Wireless::StateMachine;

Wireless wlan{};

WiFiServer server(80);

mtech::Connections<5> clients{};
mtech::Connections<3> streams{};

mtech::LEDMatrix matrix{};

void setup()
{
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        delay(20); // wait for serial port to connect. Needed for native USB port only
    }

    pinMode(led, OUTPUT);      // set the LED pin mode

    // init ADC pins
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);

    matrix.setup();

    // check for the WiFi module:
    if (!Wireless::checkModule()) {
        Serial.println("WiFi module is not available");
        // don't continue
        while (true) { delay(20); }
    }

    STM::stateMap[Wireless::State::Idle] = [](Wireless *w)
    {
        w->connect(SSID, PASSWORD);
        Serial.println("Wireless is idle");
    };
    STM::stateMap[Wireless::State::Connecting] = [](Wireless *w)
    {
        if (w->elapsed() % 1000 < 950) {
            return;
        }
        matrix.frown();
        Serial.println("Connecting to WiFi...");
        STM::stateMap[Wireless::State::Connecting] = nullptr;
    };
    STM::stateMap[Wireless::State::Connected] = [](Wireless *w)
    {
        auto client = server.available();
        if (!client || clients.capacity() == clients.size()) {
            Serial.println("Wireless::Connected: client not available");
            return;
        }
        clients.push(client);
        if (client.connected()) {
            Serial.println("New client connected");
            Serial.print("Client IP: ");
            Serial.println(client.remoteIP());
        }
    };
}

void loop()
{
    processWlan();
    matrix.loop();
    processClients();
    processStreams();
}

void processWlan()
{
    auto state = wlan.getState();
    if (auto ns = wlan.loop(); ns != state) {
        switch (ns) {
            case Wireless::State::Connected:matrix.face();
                Serial.println("Connected to WiFi");
                server.begin();
                matrix
                break;
            case Wireless::State::Disconnected:
                matrix.frown();
                Serial.println("Disconnected from WiFi");
                break;
        }
    }
}

void processClients()
{
    for (auto &client : clients) {
        if (!client.connected()) {
            Serial.println("Client disconnected");
            clients.removeClient(client);
            continue;
        }
        if (!client.available()) {
            continue;
        }
        processClient(client);
    }
}

void processClient(WiFiClient &client)
{
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.available()) {             // if there's bytes to read from the client,
        char c = static_cast<char>(client.read()); // read a byte, then
        Serial.write(c);                    // print it out to the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();
                // the content of the HTTP response follows the header:
                client.print(indexHtml);
                // The HTTP response ends with another blank line:
                client.println();
                // break out of the while loop:
                break;
            } else {      // if you got a newline, then clear currentLine:
                currentLine = "";
            }
        } else if (c != '\r') {    // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
        }

        // Check if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /adc")) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/event-stream");
            client.println("Connection: keep-alive");
            client.println("Cache-Control: no-cache");
            client.println();
            streams.push(client);
            break;
        }
    }
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
    const auto& [adc0, adc1, adc2, adc3] = measure();
    for (auto &stream: streams) {
        if (!stream.connected()) {
            Serial.println("Stream disconnected");
            streams.removeClient(stream);
            continue;
        }
        stream.print("data: [");
        stream.print(adc0);
        stream.print(", ");
        stream.print(adc1);
        stream.print(", ");
        stream.print(adc2);
        stream.print(", ");
        stream.print(adc3);
        stream.println("]");
        stream.println();
    }
}


