/*

LCD Buffer

*/

#ifndef __LcdBuffer__
#define __LcdBuffer__

#ifndef UNIT_TESTING  
    // #include "Arduino.h"
    // #include "src/LiquidCrystalFast/LiquidCrystalFast.h" // Import the LCD Fast library
#else

    #include "test-mpc/String.h"

#endif

#include "fake/LiquidCrystalFast.h"
#include "fake/Serial.h"
#include <stdint.h>
#include <string>

class LcdBuffer {
    private:
        const uint8_t columns ; // width of the LCD
        const uint8_t rows; // height of the LCD
        char ** buffer;
        char ** display;
        LiquidCrystalFast * pLcd; // pointer to the lcd object

    public:
        explicit LcdBuffer( LiquidCrystalFast * lcd, uint8_t width = 16, uint8_t height = 2 ); // constructor
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



        std::string GetBufferContents();
        std::string GetDisplayContents();

};

#endif
