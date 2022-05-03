// button.h V0.6 220503 qrt@qland.de

#pragma once

#include <Arduino.h>
#include "Adafruit_TinyUSB.h"
// #include "pico/stdlib.h"

#define BUTTON_LEFT_PIN             16              // pin button 0
#define BUTTON_RIGHT_PIN            17              //            1

#define BUTTON_LOGIC                0               // button logic, 0 negative, 1 positive
#define DBCYCLES                    16              // 40 ms / 2.5 ms (mouse report interval in encoder.h)                    
#define NUMBUT                      2               // number of buttons

typedef struct button{
    uint32_t pinMask;
    uint8_t butMask;
    uint8_t pc, rc;
    uint8_t press;
}button;

class Button
{
    public:
        static void init();
        static bool changed(uint8_t *buttons);

    private:        
        static void poll(struct button *b, uint32_t gpio);
        static struct button but[]; 
};
