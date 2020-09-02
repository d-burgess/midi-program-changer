/*

Click Button

*/

#include "ClickButton.h"

ClickButton::ClickButton( uint8_t p, uint8_t m ) {
    pin = p;
    pinMode = m;
    ::pinMode( pin, pinMode );
}; // constructor

ClickButton::~ClickButton() {

};

bool ClickButton::getSwitchPosition() {

    if ( pollingPeriodHasBeenReached() ) {

        if ( switchIsDown() ) {
            if ( downCount > debounceLimit ) { // switch constantly down for debounce period
                currentStatus = SWITCH_DOWN;
                downCount = 0;
            }
            downCount++;
            upCount = 0;
        }

        if ( switchIsUp() ) { 
            if ( upCount > debounceLimit ) { // switch constantly up for debounce period
                upCount = 0;
                currentStatus = SWITCH_UP;
            }
            upCount++;
            downCount = 0;
        }

    }

    if ( statusChangeAllowed() ) {
        lastStatus = currentStatus;
    }
    
    return lastStatus;

};

bool ClickButton::pollingPeriodHasBeenReached() {
    unsigned long currentMicros = micros();
    if ( currentMicros - previousMicros >= pollRateMicros ) {
        previousMicros = micros();
        return true;
    } else {
        return false;
    }
}

bool ClickButton::switchIsDown() {
    return ( ::digitalRead( pin ) == LOW ) ? true : false;
}

bool ClickButton::switchIsUp() {
    return ( ::digitalRead( pin ) == HIGH ) ? true : false;
}

bool ClickButton::statusChangeAllowed() {
    return ( currentStatus == lastStatus ) ? false : true;
}

const uint8_t ClickButton::getEvent() {

    bool currentSwitchPosition = getSwitchPosition();
    uint8_t returnEvent = 0;

    if ( lastEventSwitchPosition != currentSwitchPosition ) {

        lastEventSwitchPosition = currentSwitchPosition;

        if ( currentSwitchPosition == SWITCH_DOWN ) {
            timeBetweenDownEventsMillis = millis() - lastDownEventMillis;
            lastDownEventMillis = millis();
            if ( timeBetweenDownEventsMillis < doubleClickLimitMillis ) {
                if ( eventCanBeSecondClick ) {
                    eventCanBeSecondClick = false;
                    returnEvent = EVENT_DOUBLE_CLICK;
                } else {
                    eventCanBeSecondClick = true;
                    returnEvent = EVENT_CLICK;
                }
            } else {
                eventCanBeSecondClick = true;
                returnEvent =  EVENT_CLICK;
            }

        }

        if ( currentSwitchPosition == SWITCH_UP ) {
            holdTimeMillis = millis() - lastDownEventMillis;
            if ( holdTimeMillis > holdLimitMillis ) {
                returnEvent = EVENT_RELEASE;
            } else {
                returnEvent = EVENT_RELEASE;
            }
            holdTimeMillis = 0;
            canCheckForHold = true;
            canCheckForHoldRepeat = false;
            holdRepeatRateMillis = holdRepeatRateInitialMillis;
        }

    } else {

        if ( currentSwitchPosition == SWITCH_DOWN && canCheckForHold ) {
            holdTimeMillis = millis() - lastDownEventMillis;
            if ( holdTimeMillis > holdLimitMillis ) {
                canCheckForHold = false;
                returnEvent = EVENT_HOLD;
                canCheckForHoldRepeat = true;
                lastRepeatEventMillis = millis();
            }
        }

        if ( currentSwitchPosition == SWITCH_DOWN && canCheckForHoldRepeat ) {
            holdTimeMillis = millis() - lastRepeatEventMillis;
            if ( holdTimeMillis > holdRepeatRateMillis ) {
                returnEvent = EVENT_HOLD_REPEAT;
                lastRepeatEventMillis = millis();
                if ( holdRepeatRateMillis > holdRepeatRateFloorMillis ) {
                    holdRepeatRateMillis -= holdRepeatRateSpeedUpMillis;
                }
            }
        }

    }

    return returnEvent;

}

const unsigned long ClickButton::getDoubleClickLimitMillis() {
    return doubleClickLimitMillis;
}