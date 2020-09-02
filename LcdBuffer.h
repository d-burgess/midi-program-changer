/*

LCD Buffer

*/

#ifndef __LcdBuffer__
#define __LcdBuffer__


#ifndef TEENSYDUINO
    // #include "fake/Serial.h"
    // #include "fake/HardwareSerial.h"
    #include "fake/LiquidCrystalFast.h"
    // #include "fake/String.h"
    // extern Serial_ Serial;
#else
    #include "Arduino.h"
    #include "src/LiquidCrystalFast/LiquidCrystalFast.h" // Import the LCD Fast library
#endif

//custom chars for lcd
#define LCD_CHAR_FREEZE 0x0
#define LCD_CHAR_THRU 0x1
#define LCD_CHAR_EDIT 0x2
#define LCD_CHAR_NOTE 0x3
#define LCD_CHAR_SAVED 0x4
#define LCD_CHAR_CHANNEL 0x5
#define LCD_CHAR_PROGRAM 0x6
#define LCD_CHAR_ARROW 0x7
#define LCD_CHAR_NO_CHAR -1

typedef uint8_t byte;

#include "Logger.h"

#include <stdint.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <vector>

class LcdBuffer {
    private:
        const uint8_t columns; // width of the LCD
        const uint8_t rows; // height of the LCD
        char ** buffer;
        char ** display;
        LiquidCrystalFast * pLcd; // pointer to the lcd object
        Logger * pLog; // pointer to the log object
        void createCustomChars();
        void createBufferAndDisplay();
        std::vector<std::vector<int>> pCustomCharBuffer;
        void getByteAsString( char * ref, uint8_t aByte );
        void getSpacePaddedString( char * ref, uint8_t length, const char * msg );
        void fixedLengthString( char * ref, const char * msg, uint8_t length );
    public:
        explicit LcdBuffer( 
            LiquidCrystalFast * lcd, 
            Logger * log,
            uint8_t width = 16, 
            uint8_t height = 2
        ); // constructor
        ~LcdBuffer();
        void UpdateBuffer( 
            const uint8_t row, 
            const char * message, 
            const uint8_t column = 0, 
            uint8_t length = 255
        );
        void ReportBufferContents();
        void ReportDisplayContents();
        void UpdateLcdFromBuffer();
        uint8_t GetColumns();
        uint8_t GetRows();



        void GetBufferContents( char * ref );
        void GetDisplayContents( char * ref );
        uint8_t GetSize();


        void cursor();
        void setCursor( uint8_t x, uint8_t y );
        void blink();
        void noBlink();
        void noCursor();

        void writeCustomChar( int8_t c, uint8_t x, uint8_t y );

};

#endif
