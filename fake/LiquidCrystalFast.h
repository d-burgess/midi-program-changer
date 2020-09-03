#ifndef LiquidCrystalFast_h
#define LiquidCrystalFast_h

#include <stdint.h>

class LiquidCrystalFast {
    public:
        explicit LiquidCrystalFast();
        explicit LiquidCrystalFast( int pinRS, int pinRW, int pinE, int pinD4, int pinD5, int pinD6, int pinD7 );
        ~LiquidCrystalFast();
        bool initialised { false };
        virtual void begin( int c, int r );
        virtual void setCursor( int x, int y );
        virtual void clear();
        virtual void print( char c );
        virtual void cursor();
        virtual void setCursor( uint8_t x, uint8_t y );
        virtual void blink();
        virtual void noBlink();
        virtual void noCursor();
        virtual void createChar( uint8_t c, uint8_t b[ 8 ] );
        virtual void write( uint8_t b );
};

#endif
