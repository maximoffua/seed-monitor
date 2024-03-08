//
// Created by smaximov on 02.03.24.
//

#ifndef SEEDER_CONFIG_HPP
#define SEEDER_CONFIG_HPP
#include "Arduino.h"

constexpr char HOSTNAME[] = "pimx";
constexpr char ssid[] = "amarantaPilot";
constexpr char pass[] = "agromaranta";

constexpr auto led =  LED_BUILTIN;


#ifdef WIFI_SSID
constexpr auto SSID = WIFI_SSID;
#else
constexpr auto SSID = "";
#endif

#ifdef WIFI_PASSWORD
constexpr auto PASSWORD = WIFI_PASSWORD;
#else
constexpr auto PASSWORD = "";
#endif


#endif //SEEDER_CONFIG_HPP
