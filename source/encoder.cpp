// encoder.cpp V0.6 220503 qrt@qland.de

#include "encoder.h"

int8_t Encoder::xdelta, Encoder::ydelta;
int8_t Encoder::xlast, Encoder::ylast;

void Encoder::init()
{
    pinMode(X_PLU_PIN, INPUT);
    pinMode(X_MIN_PIN, INPUT);
    pinMode(Y_PLU_PIN, INPUT);
    pinMode(Y_MIN_PIN, INPUT);
}

#if DEBOUNCE

void Encoder::read(int8_t *x, int8_t *y)
{
    *x = xdelta;
    xdelta = 0;

    *y = ydelta;
    ydelta = 0;
}

#if ENCRESOL == 0
    int8_t encTab[] = { 0, 0, -1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, -1, 0, 0 };         // encoder half resolution
#elif ENCRESOL == 1
    int8_t encTab[] = { 0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0 };       //         full
#endif

void Encoder::poll()
{    
    uint32_t gpio = gpio_get_all();

    xlast = (xlast << 2)  & 0x0f;
    if(gpio & 1<<X_PLU_PIN) xlast |= 2;
    if(gpio & 1<<X_MIN_PIN) xlast |= 1;    
    xdelta += encTab[xlast];

    ylast = (ylast << 2)  & 0x0f;
    if(gpio & 1<<Y_PLU_PIN) ylast |= 2;
    if(gpio & 1<<Y_MIN_PIN) ylast |= 1;    
    ydelta += encTab[ylast];
}

#else

void Encoder::read(int8_t *x, int8_t *y)
{ 
    int8_t xval = xdelta;
    int8_t yval = ydelta;

    #if ENCSTEPS == 1
        xdelta = 0; 
        ydelta = 0; 
    #elif ENCSTEPS == 2
        xdelta = xval & 1; 
        xval >>= 1; 
        ydelta = yval & 1; 
        yval >>= 1; 
    #elif ENCSTEPS == 4
        xdelta = xval & 3; 
        xval >>= 2; 
        ydelta = yval & 3; 
        yval >>= 2; 
    #endif

    *x = xval;
    *y = yval;
}

void Encoder::poll()
{    
    uint32_t gpio = gpio_get_all();        

    int8_t xnew = 0;
    if(gpio & 1<<X_PLU_PIN) xnew = 3;
    if(gpio & 1<<X_MIN_PIN) xnew ^= 1;    
    int8_t xdif = xlast - xnew;

    if(xdif & 1){
        xlast = xnew;
        xdelta += (xdif & 2) - 1;
    }

    int8_t ynew = 0;
    if(gpio & 1<<Y_PLU_PIN) ynew = 3;
    if(gpio & 1<<Y_MIN_PIN) ynew ^= 1;    
    int8_t ydif = ylast - ynew;

    if(ydif & 1){
        ylast = ynew;
        ydelta += (ydif & 2) - 1;
    }
}

#endif