/*

LCD Buffer

*/

#include "LcdBuffer.h"
#include <cstring>
#include <stdio.h>

#ifndef TEENSYDUINO
    #include "fake/Serial.h"
    extern Serial_ Serial;
#else
    #include "Arduino.h"
#endif

LcdBuffer::LcdBuffer( LiquidCrystalFast * lcd, uint8_t width, uint8_t height ) 
    // constructor
    // member initializer list, defaults to 16x2 LCD
    : pLcd{ lcd },
    columns { width }, 
    rows { height }, 
    buffer { new char*[ height ] }, 
    display { new char*[ height ] }
    {
    uint8_t i, j;

    pLcd->begin( columns, rows ); // tell the LCD that it is a 16x2 LCD
    pLcd->setCursor( 0, 0 );
    pLcd->clear();

    // create the buffer
    for ( i = 0; i < rows; i++ ) {
        buffer[ i ] = new char[ columns + 1 ];
        display[ i ] = new char[ columns + 1 ];
    }

    // set the buffer and display to spaces and terminate strings
    for ( i = 0; i < rows; ++i ) {
        for ( j = 0; j < columns; ++j ) {
            buffer[ i ][ j ] = ' ';
            display[ i ][ j ] = ' ';
        }
        buffer[ i ][ j ] = '\0';
        display[ i ][ j ] = '\0';
    }

}

LcdBuffer::~LcdBuffer() {
    uint8_t i;

    // delete the buffer and display
    for ( i = 0; i < rows; i++ ) {
        delete buffer[ i ];
        delete display[ i ];
    }

}

void LcdBuffer::UpdateBuffer( const uint8_t row, const char * message, const uint8_t column, uint8_t length ) {

    char output[ columns + 1 ];

    // create a string from message that is the length of the column width
    if ( strlen( message ) < columns ) {
        // pad with spaces at end
        char byteAsString[ 4 ];
        snprintf( byteAsString, 4, "%d", columns );
        char formatter[ 7 ];
        formatter[ 0 ] = '\0';
        strcat( formatter, "%-" );
        strcat( formatter, byteAsString );
        strcat( formatter, "s" );
        sprintf( output, formatter, message );
    } else if ( strlen( message ) > columns ) {
        // limit to the column width
        memcpy( output, &message[0], columns );
        output[ columns ] = '\0';
    } else {
        strcpy( output, message );
    }

    if ( ( length + column ) > columns ) {
        length = columns - column;
    }

    // update buffer 
    if ( row <= rows ) {
        memcpy( &buffer[ row ][ column ], &output[ 0 ], length );
    }

}

void LcdBuffer::ReportBufferContents() {
    for ( uint8_t i = 0; i < rows; ++i ) {
        char * bufferRow = &buffer[ i ][ 0 ];
        // Serial.print( "Buffer contents row " );
        // Serial.print( i );
        // Serial.print( ": |" );
        // Serial.print( bufferRow );
        // Serial.println( "|" );
    }

}

void LcdBuffer::ReportDisplayContents() {
    uint8_t i;
    for ( i = 0; i < rows; i++ ) {
        char * displayRow = &display[ i ][ 0 ];
        // Serial.print( "Display contents row " );
        // Serial.print( i );
        // Serial.print( ": |" );
        // Serial.print( displayRow );
        // Serial.println( "|" );
    }

}

void LcdBuffer::UpdateLcdFromBuffer() {
    uint8_t i, j;
    //extern LiquidCrystalFast myLcd;

    for ( i = 0; i < rows; i++ ) {
        // update lcd by element
        for ( j = 0; j < columns; j++ ) {
            // Serial.print( "Comparing row " );
            // Serial.print( i );
            // Serial.print( ", column " );
            // Serial.print( j );
            // Serial.print( ": |" );
            // Serial.print( buffer[ i ][ j ] );
            // Serial.print( "| to |" );            
            // Serial.print( display[ i ][ j ] );
            // Serial.print( "|: " );
            if ( display[ i ][ j ] != buffer[ i ][ j ] ) {
                // Serial.println( "NO MATCH, update" );
                pLcd->setCursor ( j, i );
                char charToPrint = buffer[ i ][ j ];
                pLcd->print( charToPrint );
            } else {
                // Serial.println( "MATCHES, no update" );
            }
        }

        // update display variable by row
        memcpy( &display[ i ][ 0 ], &buffer[ i ][ 0 ], 16 );
    }

}

std::string LcdBuffer::GetBufferContents()  {
    std::string returnValue{ "" };
    for ( uint8_t i = 0; i < rows; ++i ) {
        char * bufferRow = &buffer[ i ][ 0 ];
        returnValue.append( bufferRow );
    }

    return returnValue;

};

std::string LcdBuffer::GetDisplayContents() {
    std::string returnValue{ "" };
    for ( uint8_t i = 0; i < rows; ++i ) {
        char * displayRow = &display[ i ][ 0 ];
        returnValue.append( displayRow );
    }

    return returnValue;

};