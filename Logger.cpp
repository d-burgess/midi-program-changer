#include "Logger.h"

Logger::Logger( usb_serial_class * pS )
    : pSerial ( pS )
{
    
} // constructor

Logger::~Logger() {

}

void Logger::print( const char * msg ) {
    pSerial->print( msg );
}

void Logger::print( const char c ) {
    pSerial->print( c );
}

void Logger::print( const uint8_t i ) {
    pSerial->print( i );
}

void Logger::print( const size_t s ) {
    pSerial->print( s );
}

void Logger::print( const int i ) {
    pSerial->print( i );
}

void Logger::print( const bool b ) {
    ( b ) ? pSerial->print( "TRUE" ) : pSerial->print( "FALSE" );
}

void Logger::println( const char * msg ) {
    pSerial->println( msg );
}