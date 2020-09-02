#ifndef LiquidCrystalFast_h
#define LiquidCrystalFast_h

#include <stdint.h>

class LiquidCrystalFast {
    public:
        LiquidCrystalFast( uint8_t pinRS, uint8_t pinRW, uint8_t pinE, uint8_t pinD4, uint8_t pinD5, uint8_t pinD6, uint8_t pinD7 );
        virtual ~LiquidCrystalFast();
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
        virtual void createChar( uint8_t c, uint8_t b[ 8 ] );
        virtual void write( uint8_t b );


};

#endif
