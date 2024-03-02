//
// Created by smaximov on 22.02.24.
//

#ifndef SEEDER_LEDMATRIX_HPP
#define SEEDER_LEDMATRIX_HPP

#include "Arduino_LED_Matrix.h"

namespace MTech {
static ArduinoLEDMatrix matrix{};

// bitmap for a heart symbol
uint8_t frame[8][12] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void lefteye() {
    //Left eye
    frame[1][3] = 1;
    frame[1][4] = 1;
    frame[2][3] = 1;
    frame[2][4] = 1;
}

void righteye() {
    //Right eye
    frame[1][8] = 1;
    frame[1][9] = 1;
    frame[2][8] = 1;
    frame[2][9] = 1;
}

void wink() {
    //Wink with the left eye
    frame[1][3] = 0;
    frame[1][4] = 0;
    frame[2][3] = 1;
    frame[2][4] = 1;
}

void mouth() {
    //Mouth
    frame[5][3] = 1;
    frame[5][9] = 1;
    frame[6][3] = 1;
    frame[6][4] = 1;
    frame[6][5] = 1;
    frame[6][6] = 1;
    frame[6][7] = 1;
    frame[6][8] = 1;
    frame[6][9] = 1;
    frame[5][4] = 0;
    frame[5][5] = 0;
    frame[5][6] = 0;
    frame[5][7] = 0;
    frame[5][8] = 0;
}

void frown() {
    frame[5][3] = 1;
    frame[5][9] = 1;
    frame[6][3] = 0;
    frame[6][4] = 0;
    frame[6][5] = 0;
    frame[6][6] = 0;
    frame[6][7] = 0;
    frame[6][8] = 0;
    frame[6][9] = 0;
    frame[5][3] = 1;
    frame[5][4] = 1;
    frame[5][5] = 1;
    frame[5][6] = 1;
    frame[5][7] = 1;
    frame[5][8] = 1;
    frame[5][9] = 1;
    frame[6][3] = 1;
    frame[6][9] = 1;
}
} // namespace MTech


#endif //SEEDER_LEDMATRIX_HPP
