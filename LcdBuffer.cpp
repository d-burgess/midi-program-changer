/*

LCD Buffer

*/

#include "LcdBuffer.h"

LcdBuffer::LcdBuffer( LiquidCrystalFast * lcd, Logger * logger, uint8_t width, uint8_t height ) 
    // constructor
    // member initializer list, defaults to 16x2 LCD
    : columns ( width ), 
    rows ( height ), 
    buffer ( new char*[ height ] ), 
    display ( new char*[ height ] ),
    pLcd ( lcd ),
    pLog ( logger )
    {

    pLcd->begin( columns, rows ); // tell the LCD that it is a XxY LCD
    pLcd->setCursor( 0, 0 );
    pLcd->clear();

    createBufferAndDisplay();

    createCustomChars();

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
    // create a space padded string from message that is the length of the column width
    char output[ columns + 1 ];
    fixedLengthString( output, message, columns );

    // determine how much is needed for all columns to be copied to buffer
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
        pLog->print( "Buffer contents row " );
        pLog->print( i );
        pLog->print( ": |" );
        pLog->print( bufferRow );
        pLog->println( "|" );
    }
}

void LcdBuffer::ReportDisplayContents() {
    uint8_t i;
    for ( i = 0; i < rows; i++ ) {
        char * displayRow = &display[ i ][ 0 ];
        pLog->print( "Display contents row " );
        pLog->print( i );
        pLog->print( ": |" );
        pLog->print( displayRow );
        pLog->println( "|" );
    }
}

void LcdBuffer::UpdateLcdFromBuffer() {
    uint8_t i, j;
    for ( i = 0; i < rows; i++ ) {
        // update lcd by element
        for ( j = 0; j < columns; j++ ) {
            // pLog->print( "Comparing row " );
            // pLog->print( i );
            // pLog->print( ", column " );
            // pLog->print( j );
            // pLog->print( ": |" );
            // pLog->print( buffer[ i ][ j ] );
            // pLog->print( "| to |" );            
            // pLog->print( display[ i ][ j ] );
            // pLog->print( "|: " );
            if ( display[ i ][ j ] != buffer[ i ][ j ] ) {
                // pLog->println( "NO MATCH, update" );
                pLcd->setCursor ( j, i );
                char charToPrint = buffer[ i ][ j ];
                pLcd->print( charToPrint );
            } else {
                // pLog->println( "MATCHES, no update" );
            }
        }

        // update display variable by row
        memcpy( &display[ i ][ 0 ], &buffer[ i ][ 0 ], columns );
    }
}

void LcdBuffer::GetBufferContents( char * ref ) {
    * ref = '\0';
    for ( uint8_t i = 0; i < rows; ++i ) {
        char * bufferRow = &buffer[ i ][ 0 ];
        strcat( ref, bufferRow );
        strcat( ref, "\n" );
    }
}

void LcdBuffer::GetDisplayContents( char * ref ) {
    * ref = '\0';
    for ( uint8_t i = 0; i < rows; ++i ) {
        char * displayRow = &display[ i ][ 0 ];
        strcat( ref, displayRow );
        strcat( ref, "\n" );
    }
}

uint8_t LcdBuffer::GetColumns() {
    return columns;
}

uint8_t LcdBuffer::GetRows() {
    return rows;
}

uint8_t LcdBuffer::GetSize() {
    return GetColumns() * ( GetRows() + 1 ) + rows;
}

void LcdBuffer::cursor() {
     pLcd->cursor();
}

void LcdBuffer::setCursor( uint8_t x, uint8_t y ) {
     pLcd->setCursor( x, y );
}

void LcdBuffer::blink() {
     pLcd->blink();
}

void LcdBuffer::noBlink() {
     pLcd->noBlink();
}

void LcdBuffer::noCursor() {
     pLcd->noCursor();
}

void LcdBuffer::writeCustomChar( int8_t c, uint8_t x, uint8_t y ) {

    // for (size_t i = 0; i < rows; ++i )
    // {
    //     for (size_t j = 0; j < columns; ++j )
    //     {
    //         pLog->print( i );
    //         pLog->print( "," );
    //         pLog->print( j );
    //         pLog->print( ": " );
    //         pLog->print( pCustomCharBuffer[ x ][ y ] );
    //         pLog->println( "" );
    //         }
    // }
    
    if ( pCustomCharBuffer[ x ][ y ] != c ) {
        pCustomCharBuffer[ x ][ y ] = c;
        setCursor( x, y );
        pLcd->write( byte( c ) );
    } 
}

void LcdBuffer::createCustomChars() {
    // setup custom chars
    uint8_t freeze[ 8 ] {
        0b11111,
        0b10001,
        0b10111,
        0b10001,
        0b10111,
        0b10111,
        0b10111,
        0b11111
    };
    pLcd->createChar( LCD_CHAR_FREEZE, freeze );

    uint8_t thru[ 8 ] {
        0b11111,
        0b10001,
        0b11011,
        0b11011,
        0b11011,
        0b11011,
        0b11011,
        0b11111
    };
    pLcd->createChar( LCD_CHAR_THRU, thru );

    uint8_t edit[ 8 ] {
        0b11111,
        0b10001,
        0b10111,
        0b10001,
        0b10111,
        0b10111,
        0b10001,
        0b11111
    };
    pLcd->createChar( LCD_CHAR_EDIT, edit );

    uint8_t note[ 8 ] {
        0b00010,
        0b00011,
        0b00010,
        0b00010,
        0b01110,
        0b11110,
        0b11110,
        0b01100
    };
    pLcd->createChar( LCD_CHAR_NOTE, note );

    uint8_t saved[ 8 ] {
        0b11111,
        0b11001,
        0b10111,
        0b11011,
        0b11101,
        0b11101,
        0b10011,
        0b11111
    };
    pLcd->createChar( LCD_CHAR_SAVED, saved );

    uint8_t channel[ 8 ] {
        0b00110,
        0b01001,
        0b01000,
        0b01000,
        0b01001,
        0b00110,
        0b00000,
        0b00000
    };
    pLcd->createChar( LCD_CHAR_CHANNEL, channel );

    uint8_t program[ 8 ] {
        0b01110,
        0b01001,
        0b01001,
        0b01110,
        0b01000,
        0b01000,
        0b00000,
        0b00000
    };
    pLcd->createChar( LCD_CHAR_PROGRAM, program );

    uint8_t arrow[ 8 ] {
        0b00000,
        0b00100,
        0b00010,
        0b11111,
        0b00010,
        0b00100,
        0b00000,
        0b00000
    };
    pLcd->createChar( LCD_CHAR_ARROW, arrow );

    // create array to keep track of custom char positions and values
    pCustomCharBuffer = std::vector < std::vector <uint8_t> >( columns, std::vector <uint8_t> ( rows, LCD_CHAR_NO_CHAR ) );
}

void LcdBuffer::createBufferAndDisplay() {
    // create the buffer and display
    uint8_t i, j;

    for ( i = 0; i < rows; ++i ) {
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

void LcdBuffer::getByteAsString( char * ref, uint8_t aByte ) {
    snprintf( ref, 4, "%d", aByte );
}

void LcdBuffer::getSpacePaddedString( char * ref, uint8_t length, const char * msg ) {
    char byteAsString[ 4 ];
    getByteAsString( byteAsString, length );
    char formatter[ 7 ];
    formatter[ 0 ] = '\0';
    strcat( formatter, "%-" );
    strcat( formatter, byteAsString );
    strcat( formatter, "s" );
    sprintf( ref, formatter, msg );
}

void LcdBuffer::fixedLengthString( char * ref, const char * msg, uint8_t length ) {
    // create a string from message that is the length of the column width
    if ( strlen( msg ) < length ) {
        // pad with spaces at end
        getSpacePaddedString( ref, length, msg );
    } else if ( strlen( msg ) > length ) {
        // limit to the column width
        memcpy( ref, &msg[0], length );
        ref[ length ] = '\0';
    } else {
        strcpy( ref, msg );
    }
};