#include "HardwareSerial.h"

usb_serial_class::usb_serial_class() {
    initialised = true;
}

usb_serial_class::~usb_serial_class() {

}

usb_serial_class::operator bool() {
    return true;
}

void usb_serial_class::print( const char * c ) {
    std::cout<<c;
}

void usb_serial_class::print( uint8_t i ) {
    std::cout<<unsigned(i);
}

void usb_serial_class::print( time_t t ) {
    std::cout<<t<<std::endl;
}

void usb_serial_class::print( const std::string s ) {
    std::cout<<s<<std::endl;
}

void usb_serial_class::print( const size_t s ) {
    std::cout<<s<<std::endl;
}

void usb_serial_class::print( const int i ) {
    std::cout<<i<<std::endl;
}

void usb_serial_class::println( const char * c ) {
    std::cout<<c<<std::endl;
}

void usb_serial_class::print( const char& c ) {
    std::cout<<c<<std::endl;
}

void usb_serial_class::println( uint8_t i ) {
    std::cout<<unsigned(i)<<std::endl;
}

void usb_serial_class::begin( int i ) {

}

int usb_serial_class::available() {
    return 0;
}

uint8_t usb_serial_class::read() {
    return 0;
}

extern usb_serial_class Serial;
