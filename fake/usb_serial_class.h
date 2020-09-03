#ifndef usb_serial_class_h
#define usb_serial_class_h

#include <stdint.h>
#include <iostream>
// #include <string>
#include <cstddef>

class usb_serial_class {
    public:
        explicit usb_serial_class();
        ~usb_serial_class();
        bool initialised { false };
        explicit operator bool();
        virtual void print( const char * c );
        virtual void print( uint8_t i );
        virtual void print( time_t t );
        // virtual void print( const std::string s );
        virtual void print( const char& c );
        virtual void print( const std::size_t s );
        virtual void print( const int i );
        virtual void println( const char * c );
        virtual void println( uint8_t i );
        virtual void begin( int i );
        virtual int available();
        virtual uint8_t read(); 
};

#endif
