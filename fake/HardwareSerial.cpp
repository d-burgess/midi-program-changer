#include "HardwareSerial.h"

HardwareSerial::HardwareSerial() {
    initialised = true;
    std::cout<<"HardwareSerial constructor called"<<std::endl;
}

HardwareSerial::~HardwareSerial() {
    std::cout<<"HardwareSerial destructor called"<<std::endl;
}

HardwareSerial::operator bool() {
    return true;
}

void HardwareSerial::print( const char * c ) {
    std::cout<<c;
}

void HardwareSerial::print( uint8_t i ) {
    std::cout<<unsigned(i);
}

void HardwareSerial::print( time_t t ) {
    std::cout<<t;
}
/* 
void HardwareSerial::print( const std::string s ) {
    std::cout<<s;
}
*/
void HardwareSerial::print( const size_t s ) {
    std::cout<<s;
}

void HardwareSerial::print( const int i ) {
    std::cout<<i;
}

void HardwareSerial::println( const char * c ) {
    std::cout<<c<<std::endl;
}

void HardwareSerial::print( const char& c ) {
    std::cout<<c;
}

void HardwareSerial::println( uint8_t i ) {
    std::cout<<unsigned(i)<<std::endl;
}

void HardwareSerial::begin( int i ) {
    std::cout<<"HardwareSerial begin called with "<<i<<std::endl;
}

int HardwareSerial::available() {
    return 0;
}

uint8_t HardwareSerial::read() {
    return 0;
}
