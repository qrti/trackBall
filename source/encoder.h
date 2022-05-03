// encoder.h V0.6 220503 qrt@qland.de

#pragma once

#include <Arduino.h>
// #include "pico/stdlib.h"

#define DEBOUNCE                false       // false | true     debounce for mechanical and some optical encoders

#if DEBOUNCE
    #define ENCRESOL            0           // 0 | 1            half or full resolution
#else
    #define ENCSTEPS            1           // 1 | 2 | 4        single, dual or quadruple step encoder
#endif

#define POLLCYCLE               250         // poll cycle       in us
#define POLLREPEAT              10          // poll repeat      mouse report interval = POLLREPEAT * POLLCYCLE (here 2.5 ms)

#define SWAP_XY                 true        // swap x and y axis
#define SWAP_DIR_X              false       //      x direction
#define SWAP_DIR_Y              true        //      y

#if SWAP_XY
    #define X_0_PIN             18
    #define Y_0_PIN             19
    #define Y_1_PIN             20
    #define X_1_PIN             21
#else
    #define Y_0_PIN             18
    #define X_0_PIN             19
    #define X_1_PIN             20
    #define Y_1_PIN             21
#endif

#if SWAP_DIR_X
    #define X_MIN_PIN           X_1_PIN
    #define X_PLU_PIN           X_0_PIN
#else    
    #define X_MIN_PIN           X_0_PIN
    #define X_PLU_PIN           X_1_PIN
#endif

#if SWAP_DIR_Y
    #define Y_MIN_PIN           Y_1_PIN
    #define Y_PLU_PIN           Y_0_PIN
#else    
    #define Y_MIN_PIN           Y_0_PIN
    #define Y_PLU_PIN           Y_1_PIN
#endif

class Encoder
{
    public:
        static void init();
        static void read(int8_t *x, int8_t *y);
        static void poll();
        
    private:        
        static int8_t xdelta, ydelta;
        static int8_t xlast, ylast;
};
