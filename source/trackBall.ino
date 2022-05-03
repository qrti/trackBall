/*  trackBall.ino V0.6 220503 qrt@qland.de

    links
    - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    https://github.com/adafruit/Adafruit_TinyUSB_Arduino
    https://github.com/earlephilhower/arduino-pico

    preparation
    - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Adafruit TinyUSB Library does not work with arduino standard board manager so you have to set up a working one

    arduino IDE
    File/Preferences/Additional Board Manager URLs
    https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

    vscode arduino (or arduino IDE)
    library manager   -> Adafruit TinyUSB Library
    board manager     -> Raspberry Pi Pico/ RP2040 by Earle F. Philhower
    board config      -> Raspberry Pi Pico (Raspberry Pi Pico/RP2040)      
                         USB Stack -> Adafruit TinyUSB

    if necessary, rebuild intellisense configuration to update c_cpp_properties.json (CTRL+ALT+I)
    (vscode/file/preferences/settings/arduino/disable intelli sense auto gen -> must be UNCHECKED)

    if #include "Adafruit_TinyUSB.h" is squiggled, add to workspace settings.json
    "C_Cpp.intelliSenseEngine": "Tag Parser"

    arduino.json should look like this
    - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    "sketch": "trackBall_ard.ino",
    "board": "rp2040:rp2040:rpipico",
    "output": "../_build_arduino",
    "port": "COM4",
    "configuration": "flash=2097152_0,freq=133,opt=Small,rtti=Disabled,dbgport=Disabled,dbglvl=None,usbstack=tinyusb"
*/

#include <Arduino.h>
#include "Adafruit_TinyUSB.h"
#include "encoder.h"
#include "button.h"

enum{                               // report ID
    RID_KEYBOARD = 1,               // keyboard
    RID_MOUSE,                      // mouse
    RID_CONSUMER_CONTROL,           // media, volume etc ..
};

uint8_t const desc_hid_report[] =   // HID report descriptor using TinyUSB's template
{
    TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(RID_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(RID_MOUSE)),
    TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(RID_CONSUMER_CONTROL))
};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

void setup()
{
    // usb_hid.setPollInterval(2);
    // usb_hid.setReportDescriptor();
    // usb_hid.setStringDescriptor("TinyUSB HID Composite");

    usb_hid.begin();

    pinMode(PICO_DEFAULT_LED_PIN, OUTPUT);                      // LED
    digitalWrite(PICO_DEFAULT_LED_PIN, 1);                      // on

    while(!TinyUSBDevice.mounted())                             // wait until device mounted
        delay(1);

    Encoder::init();                                            // init encoder
    Button::init();                                             //      buttons
    digitalWrite(PICO_DEFAULT_LED_PIN, 0);                      // LED off
}

#define LEDSLOW         (0xffff >> 8)               
#define LEDFAST         (LEDSLOW >> 4)

void loop()
{
    static int8_t dx, dy;
    static uint16_t cnt;
    static uint8_t buttons;
        
    for(uint8_t i=0; i<POLLREPEAT; i++){
        Encoder::poll();
        delayMicroseconds(POLLCYCLE);        
    }

    Encoder::read(&dx, &dy);

    if(dx || dy){        
        if(TinyUSBDevice.suspended())                           // remote wakeup
            TinyUSBDevice.remoteWakeup();

        if(usb_hid.ready())
            usb_hid.mouseMove(RID_MOUSE, dx, dy);

        if((++cnt & LEDFAST) == 0)
            digitalWrite(PICO_DEFAULT_LED_PIN, digitalRead(PICO_DEFAULT_LED_PIN)^1);
    }
    else{
        if((++cnt & LEDSLOW) == 0)
            digitalWrite(PICO_DEFAULT_LED_PIN, digitalRead(PICO_DEFAULT_LED_PIN)^1);
    }

    if(Button::changed(&buttons))                               // every POLLREPEAT * POLLCYCLE us
        usb_hid.mouseReport(RID_MOUSE, buttons, 0, 0, 0, 0);    // stores buttons for mouseMove
}
