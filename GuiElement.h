/*

Gui Element

*/

#ifndef __MPCGuiElement__
#define __MPCGuiElement__

#include "Logger.h"

class GuiElement {
    private:
        Logger * pLog;
        uint8_t min;
        uint8_t max;
        uint8_t row;
        uint8_t column;
        uint8_t digits;
        uint8_t value;
    public:
        explicit GuiElement( 
            Logger * logger, 
            uint8_t mn,
            uint8_t mx,
            uint8_t r,
            uint8_t c,
            uint8_t d
        );
        ~GuiElement();
        void increment();
        void decrement();
        uint8_t getValue();
        uint8_t getRow();
        uint8_t getColumn();
        uint8_t getDigits();
};

#endif
