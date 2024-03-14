//
// Created by Sergii Maksymov on 13.03.24.
// Copyright (c) 2024 Maximoff Tech. All rights reserved.
//

#ifndef SENSOR_HPP
#define SENSOR_HPP

class Sensor {
public:
    Sensor() = default;
    explicit Sensor(int pin) : pin(pin) {}

    int loop() {
        auto v = read();
        if (v < 20 && last > 20) {
            counter++;
        }
        last = v;
        return v;
    }

    [[nodiscard]] int read() const {
        return analogRead(pin);
    }

    void setPin(uint8_t pin) {
        this->pin = pin;
    }

    [[nodiscard]] int getPin() const {
        return pin;
    }

    [[nodiscard]] int32_t count() const {
        return counter;
    }

    [[nodiscard]] uint16_t value() {
        auto c = counter;
        counter = 0;
        return last;
    }

    void setup() const {
        pinMode(pin, INPUT);
    }

private:
    uint8_t pin{A0};
    uint16_t last{};
    int32_t counter{};
};

#endif //SENSOR_HPP
