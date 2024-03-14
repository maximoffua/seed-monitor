//
// Created by smaximov on 20.02.24.
//

#include <Arduino.h>
#include <wifi/WiFiClient.hpp>
#include "r4/config.hpp"
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
    for (auto &sens : sensors) {
        sens.setup();
    }

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
        for (const auto &existing : clients) {
            if (client == existing) {
                return;
            }
        }
        for (const auto &existing : streams) {
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
    static int route{0};
    auto line = client.readStringUntil('\r');
    Serial.println(line);
    if (line.startsWith("GET / ")) {
        route = 1;
    }
    else if (line.startsWith("GET /adc ")) {
        route = 2;
    }
    readAll(client);

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
    }
    else {
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
    }
    route = 0;
}

void processStreams()
{
    for (auto &stream : streams.clients) {
        if (!stream) {
            continue;
        }
        readAll(stream);
        if (!stream.connected()) {
            Serial.println("disconnected");
            stream.stop();
            continue;
        }
        sendData(stream);
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
        auto &sens = sensors[i];
        stream.print(sens.value());
        if (i < (sizeof(sensors) / sizeof(Sensor)) - 1) {
            stream.print(',');
        }
    }
    stream.println(']');
}
