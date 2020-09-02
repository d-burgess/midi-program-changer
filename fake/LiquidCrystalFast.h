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
};

#endif
