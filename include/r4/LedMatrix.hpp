//
// Created by smaximov on 22.02.24.
//

#ifndef SEEDER_LEDMATRIX_HPP
#define SEEDER_LEDMATRIX_HPP

#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>

namespace mtech {
class LEDMatrix
{
    constexpr static auto WIDTH = 12;
    constexpr static auto HEIGHT = 8;
private:
// bitmap for a heart symbol
    uint8_t frame[HEIGHT][WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

public:
    ArduinoLEDMatrix matrix{};

    void setup()
    {
//        matrix.begin();
    }

    void loop()
    {
        matrix.next();
    }

    void draw() {
        matrix.renderBitmap(frame, HEIGHT, WIDTH);
    }

    void printText(const char *text)
    {
        matrix.beginDraw();
        matrix.stroke(0xFFFFFFFF);
        matrix.textScrollSpeed(50);
        // add the text
        matrix.textFont(Font_5x7);
        matrix.beginText(0, 1, 0xFFFFFF);
        matrix.println(text);
        matrix.endText(SCROLL_LEFT);
        matrix.endDraw();
    }

    void leftEye()
    {
        //Left eye
        frame[1][3] = 1;
        frame[1][4] = 1;
        frame[2][3] = 1;
        frame[2][4] = 1;
    }

    void rightEye()
    {
        //Right eye
        frame[1][8] = 1;
        frame[1][9] = 1;
        frame[2][8] = 1;
        frame[2][9] = 1;
    }

    void wink()
    {
        //Wink with the left eye
        frame[1][3] = 0;
        frame[1][4] = 0;
        frame[2][3] = 1;
        frame[2][4] = 1;
    }

    void mouth()
    {
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

    void frown()
    {
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

    void smile()
    {
        frame[5][3] = 1;
        frame[5][9] = 1;
        frame[6][3] = 0;
        frame[6][4] = 0;
        frame[6][5] = 0;
        frame[6][6] = 0;
        frame[6][7] = 0;
        frame[6][8] = 0;
        frame[6][9] = 0;
        frame[5][4] = 1;
        frame[5][5] = 1;
        frame[5][6] = 1;
        frame[5][7] = 1;
        frame[5][8] = 1;
    }

    void neutral()
    {
        frame[5][3] = 1;
        frame[5][9] = 1;
        frame[6][3] = 0;
        frame[6][4] = 0;
        frame[6][5] = 0;
        frame[6][6] = 0;
        frame[6][7] = 0;
        frame[6][8] = 0;
        frame[6][9] = 0;
        frame[5][4] = 0;
        frame[5][5] = 0;
        frame[5][6] = 0;
        frame[5][7] = 0;
        frame[5][8] = 0;
    }

    void off()
    {
        for (auto &i : frame)
        {
            for (auto &j : i)
            {
                j = 0;
            }
        }
    }

    void on()
    {
        for (auto &i : frame)
        {
            for (auto &j : i)
            {
                j = 1;
            }
        }
    }

    void face() {
        leftEye();
        rightEye();
        mouth();
    }
}; // class LEDMatrix
} // namespace MTech


#endif //SEEDER_LEDMATRIX_HPP
