#include "Serial.h"

Serial_::Serial_() {
    initialised = true;
}

Serial_::operator bool() {
    return true;
}

void Serial_::print( const char * c ) {
    std::cout<<c;
}

void Serial_::print( uint8_t i ) {
    std::cout<<unsigned(i);
}

void Serial_::print( time_t t ) {
    std::cout<<t<<std::endl;
}

void Serial_::print( const std::string s ) {
    std::cout<<s<<std::endl;
}

void Serial_::println( const char * c ) {
    std::cout<<c<<std::endl;
}

void Serial_::println( uint8_t i ) {
    std::cout<<unsigned(i)<<std::endl;
}

void Serial_::begin( int i ) {

}

int Serial_::available() {
    return 0;
}

uint8_t Serial_::read() {
    return 0;
}


