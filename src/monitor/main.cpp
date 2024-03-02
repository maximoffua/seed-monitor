//
// Created by smaximov on 02.03.24.
//
#include "mbed.h"

constexpr auto WAIT_TIME_MS = 500;
DigitalOut led1(LED1);

int main() {
    printf("This is the bare metal blinky example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    while (true)
    {
        led1 = !led1;
        thread_sleep_for(WAIT_TIME_MS);
    }
}