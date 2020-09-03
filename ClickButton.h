/*

Click Button

*/

#ifndef __ClickButton__
#define __ClickButton__

#define SWITCH_UP 0
#define SWITCH_DOWN 1

#define EVENT_NO_EVENT 0
#define EVENT_RELEASE 1
#define EVENT_CLICK 2
#define EVENT_DOUBLE_CLICK 3
#define EVENT_HOLD 4
#define EVENT_HOLD_REPEAT 5

#ifndef TEENSYDUINO
    #include "fake/Arduino.h"
#else
    #include "Arduino.h"
#endif

#include <stdint.h>

class ClickButton {
    private:
        uint8_t pin;
        uint8_t pinMode = INPUT_PULLUP;
        uint8_t lastStatus = SWITCH_UP;
        uint8_t downCount = 0;
        uint8_t upCount = 0;
        uint8_t currentStatus = getSwitchPosition();
        unsigned long previousMicros = 0;
        unsigned long pollRateMicros = 250;
        const uint8_t debounceLimit = 60; // 250us x 60 = 15ms
        const unsigned long holdLimitMillis = 1000;
        const unsigned long holdRepeatRateInitialMillis = 250;
        unsigned long holdRepeatRateMillis = holdRepeatRateInitialMillis;
        const unsigned long holdRepeatRateFloorMillis = 70;
        const unsigned long holdRepeatRateSpeedUpMillis = 5;
        const unsigned long doubleClickLimitMillis = 250;
        bool eventCanBeSecondClick = false;
        unsigned long timeBetweenDownEventsMillis = 0;
        unsigned long holdTimeMillis = 0;
        unsigned long lastDownEventMillis = millis();
        unsigned long lastRepeatEventMillis = millis();
        bool canCheckForHold = true;
        bool canCheckForHoldRepeat = false;
        bool lastEventSwitchPosition = SWITCH_UP;
        bool pollingPeriodHasBeenReached();
        bool switchIsDown();
        bool switchIsUp();
        bool statusChangeAllowed();
        bool getSwitchPosition();

    public:
        ClickButton( uint8_t pin, uint8_t mode );
        ~ClickButton();
        const uint8_t getEvent();
        const unsigned long getDoubleClickLimitMillis();  
};

#endif
