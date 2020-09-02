/*

Logger

*/

#ifndef __MPCLogger__
#define __MPCLogger__


#ifndef TEENSYDUINO
    // #include "fake/Serial.h"
    #include "fake/HardwareSerial.h"
    #include "fake/LiquidCrystalFast.h"
    // #include "fake/String.h"
#else
    #include "Arduino.h"
    #include "src/LiquidCrystalFast/LiquidCrystalFast.h" // Import the LCD Fast library
#endif

class Logger {
    private:
        usb_serial_class * pSerial;

    public:
        explicit Logger( usb_serial_class * pS ); // constructor
        ~Logger();
        void print( const char * msg );
        void print( const char c );
        void print( const uint8_t i );
        void print( const bool b );
        void print( const size_t i );
        void print( const int i );
        void println( const char * msg );

};

#endif
