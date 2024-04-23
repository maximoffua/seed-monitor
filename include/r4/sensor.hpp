//
// Created by Sergii Maksymov on 13.03.24.
// Copyright (c) 2024 Maximoff Tech. All rights reserved.
//

#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <Arduino.h>

class Sensor {
  static constexpr auto THRESHOLD = 8;
public:
    Sensor() = default;
    explicit Sensor(int pin) : pin(pin) {}

    int loop() {
        auto v = read();
        if (v < THRESHOLD && ts == 0) ts = millis();
        else if (v > THRESHOLD) ts = 0;
        if (v < THRESHOLD && v < last && last > THRESHOLD && duration() < 2) {
            counter++;
        }
        last = v;
        return v;
    }

    [[nodiscard]] uint16_t read() const {
        return analogRead(pin);
    }

    void setPin(uint8_t pin) {
        this->pin = pin;
    }

    [[nodiscard]] int getPin() const {
        return pin;
    }

    [[nodiscard]] uint32_t count() const {
        return counter;
    }

    [[nodiscard]] uint16_t value() {
        return last;
    }

    void setup() const {
        pinMode(pin, INPUT);
    }

    void reset() {
      last = 0;
      counter = 0;
    }

    int32_t duration() {
      return ts > 0 ? millis() - ts : 0;
    }

    void printThis(Print& out, uint8_t id) {
      out.print("A@"); out.print(id); out.print(':');
      out.print(value());
      out.print("\tC@"); out.print(id); out.print(':');
      out.print(counter);
      out.print("\tD@"); out.print(id); out.print(':');
      out.print(ts ? millis() - ts : 0);
    }

private:
    uint8_t pin{A0};
    uint16_t last{};
    uint32_t counter{};
    uint32_t ts{};
};

#endif //SENSOR_HPP
