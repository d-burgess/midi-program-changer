/*

LCD Buffer

*/

#ifndef __LcdBuffer__
#define __LcdBuffer__

#include "Arduino.h"

#include "src/LiquidCrystalFast/LiquidCrystalFast.h" // Import the LCD Fast library

// #include <memory>

class LcdBuffer {
    private:
        const uint8_t columns ; // width of the LCD
        const uint8_t rows; // height of the LCD
        char ** buffer;
        char ** display;
        LiquidCrystalFast * lcd; // pointer to the lcd object

    public:
        LcdBuffer( 
            uint8_t width = 16, 
            uint8_t height = 2,
            uint8_t pinRS = 2,
            uint8_t pinRW = 8,
            uint8_t pinE = 3,
            uint8_t pinD4 = 4,
            uint8_t pinD5 = 5,
            uint8_t pinD6 = 6,
            uint8_t pinD7 = 7
        ); // constructor

        ~LcdBuffer();

        void UpdateBuffer( 
            uint8_t row, 
            const char * message, 
            uint8_t column = 0, 
            uint8_t length = 16 
        );

        void ReportBufferContents();

        void ReportDisplayContents();

        void UpdateLcdFromBuffer();
};

#endif
