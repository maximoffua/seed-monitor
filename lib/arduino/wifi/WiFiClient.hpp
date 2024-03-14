//
// Created by Sergii Maksymov on 07.03.24.
// Copyright (c) 2024 Maximoff Tech. All rights reserved.
//

#ifndef WIFICLIENT_HPP
#define WIFICLIENT_HPP

#include <WiFiS3.h>
#include <functional>


// Maximum size of a SSID
#define WL_SSID_MAX_LENGTH 32

// Size of a MAC-address or BSSID
#define WL_MAC_ADDR_LENGTH 6

// Size of a MAC-address or BSSID
#define WL_IPV4_LENGTH 4

// Maximum size of a SSID list
#define WL_NETWORKS_LIST_MAXNUM	10

namespace mtech
{
const char* statusToString(int status);


class WiFiClient
{
public:
    struct StateMachine
    {
        using action = std::function<void(WiFiClient*)>;
        enum State
        {
            Idle,
            Connecting,
            Connected,
            Disconnected,
            count,
        };
        State state{};

        static action stateMap[];
    };
public:
    using State = StateMachine::State;
public:
    static bool checkModule();

    unsigned char
    connect(const char *ssid, const char *pass, int timeout = 15'000);

    [[maybe_unused]] static void printWiFiStatus();

    static void printMacAddress(byte mac[]);

    static void printWifiData();

    static void printCurrentNet();

    void begin();

    State loop() noexcept;

    int updateStatus();

    [[nodiscard]] size_t elapsed() const;

        [[nodiscard]] StateMachine::State getState() const;
private:
    int status{};
    size_t opStart{};
    StateMachine stm{};
};

const char* stateToString(WiFiClient::State state);

} // mtech

#endif //WIFICLIENT_HPP
