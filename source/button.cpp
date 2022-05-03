// button.cpp V0.6 220503 qrt@qland.de

#include "button.h"

struct button Button::but[NUMBUT];

void Button::init()
{
    pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

    but[0] = { 1<<BUTTON_LEFT_PIN,  1<<MOUSE_BUTTON_LEFT,  0, 0xff };
    but[1] = { 1<<BUTTON_RIGHT_PIN, 1<<MOUSE_BUTTON_RIGHT, 0, 0xff };
}

bool Button::changed(uint8_t *buttons)
{
    static uint8_t last;

    uint32_t gpio = gpio_get_all();           
    bool rv;
    *buttons = 0;

    for(uint8_t i=0; i<NUMBUT; i++){
        poll(&but[i], gpio);

        if(but[i].press)
            *buttons |= but[i].butMask;
    }

    rv = *buttons != last;
    last = *buttons;
    return rv;
}

void Button::poll(struct button *b, uint32_t gpio)
{
#if BUTTON_LOGIC == 0                       // button logic negative
    if(!(gpio & b->pinMask)){               //        pressed                      
#else
    if(gpio & b->pinMask){                  //        logic positive            
#endif    
        if(b->pc == DBCYCLES){             
            b->rc = 0;
            b->press = 1;
        }

        if(b->pc < 0xff) b->pc++;
    }
    else{                                   //        released
        if(b->rc == DBCYCLES){ 
            b->pc = 0;
            b->press = 0;
        }
        
        if(b->rc < 0xff) b->rc++;
    }
}
