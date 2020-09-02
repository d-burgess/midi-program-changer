#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <iostream>

class Serial_ {
    public:
        Serial_();
        bool initialised { false };
        explicit operator bool();
        virtual void print( const char * c );
        virtual void print( uint8_t i );
        virtual void print( time_t t );
        virtual void print( const std::string s );
        virtual void print( const char& c );
        virtual void println( const char * c );
        virtual void println( uint8_t i );
        virtual void begin( int i );
        virtual int available();
        virtual uint8_t read(); 
};

#endif
