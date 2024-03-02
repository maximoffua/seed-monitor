//
// Created by smaximov on 02.03.24.
//

#ifndef SEEDER_CONFIG_HPP
#define SEEDER_CONFIG_HPP
#include "Arduino.h"

constexpr char HOSTNAME[] = "amapi";
constexpr char ssid[] = "amarantaPilot";
constexpr char pass[] = "agromaranta";

constexpr auto led =  LED_BUILTIN;

// Maximum size of a SSID
#define WL_SSID_MAX_LENGTH 32

// Size of a MAC-address or BSSID
#define WL_MAC_ADDR_LENGTH 6

// Size of a MAC-address or BSSID
#define WL_IPV4_LENGTH 4

// Maximum size of a SSID list
#define WL_NETWORKS_LIST_MAXNUM	10

#endif //SEEDER_CONFIG_HPP
