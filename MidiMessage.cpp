/*

Midi Message

*/

#include "MidiMessage.h"

// #include "HardwareSerial.h"

MidiMessage::MidiMessage() {
    // constructor for empty midi message
    statusByte = 0;
    dataByte2 = 0;
    dataByte3 = 0;
}

MidiMessage::MidiMessage( HardwareSerial * pSerial ) {
    // constructor to get midi message from serial
    bool thirdByteFlag = false;
    bool messageComplete = false;

    do {
        if ( pSerial->available() > 0 ) { 
            uint8_t serialByte = pSerial->read(); // read a byte from serial
            if ( serialByte != 0b11111110 ) { // active sensing
                // output 8-digit binary number to serial
                // char binary[ 9 ];
                // byteToBinaryString( binary, serialByte );
                // Serial.print( binary );
                // Serial.print( " (" );
                // Serial.print( serialByte );
                // Serial.println( ")" );
            }
            if ( serialByte & STATUS_BIT ) { // Status byte received
                runningStatus = serialByte;
                thirdByteFlag = false;
            } else {
                if ( thirdByteFlag ) { // Second data byte received
                    thirdByteFlag = false;
                    dataByte3 = serialByte;
                    messageComplete = true;
                } else { // First data byte received
                    if ( !runningStatus ) { // no status byte
                        // ignore invalid data byte
                    } else {
                        // running status is valid
                        if ( runningStatus < ( PROGRAM_CHANGE << 4 ) ) { // First data byte of Note Off/On, Key Pressure or Control Change
                            thirdByteFlag = true;
                            statusByte = runningStatus;
                            dataByte2 = serialByte;
                        } else if ( runningStatus < ( PITCH_BEND << 4 ) ) { // First data byte of Program Change or Channel Pressure
                            statusByte = runningStatus;
                            dataByte2 = serialByte;
                            dataByte3 = 0;
                            messageComplete = true;
                        } else if ( runningStatus < 0xF0 ) { // First data byte of Pitch Bend
                            thirdByteFlag = true;
                            statusByte = runningStatus;
                            dataByte2 = serialByte;
                        } else { // System message, ignore
                            runningStatus = 0;
                        }
                    } // end running status not empty 
                } // end not third data byte
            } // end not header byte
        } // end serial available
    } while ( messageComplete == false );

}

MidiMessage::MidiMessage( uint8_t s, uint8_t d1, uint8_t d2 ) {
    // constructor for manual midi message
    statusByte = s;
    dataByte2 = d1;
    dataByte3 = d2;
}

MidiMessage::~MidiMessage() {
    // destructor
}

const uint8_t MidiMessage::GetStatusByte() {
    return statusByte;
};

void MidiMessage::GetMessageTypeString( char * ref ) {
    // message type is top four bits
    uint8_t messageType = GetMessageType();
    ref[ 0 ] = '\0';
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( ref, "NOTE OFF" );
            break;
        case NOTE_ON:
            strcpy( ref, "NOTE ON" );
            break;
        case KEY_PRESSURE:
            strcpy( ref, "KEY PRESSURE" );
            break;
        case CONTROL_CHANGE:
            strcpy( ref, "CONTROL CHANGE" );
            break;
        case PROGRAM_CHANGE:
            strcpy( ref, "PROGRAM CHANGE" );
            break;
        case CHANNEL_PRESSURE:
            strcpy( ref, "CHANNEL PRESSURE" );
            break;
        case PITCH_BEND:
            strcpy( ref, "PITCH BEND" );
            break;
        default:
            strcpy( ref, "UNKNOWN?" );
    }
}

void MidiMessage::GetMessageTypeChar( char * ref ) {
    uint8_t messageType = GetMessageType();
    std::string dataByte2Type;
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( ref, "NOf" );
            break;
        case NOTE_ON:
            strcpy( ref, "NOn" );
            break;
        case KEY_PRESSURE:
            strcpy( ref, "KyP" );
            break;
        case CONTROL_CHANGE:
            strcpy( ref, "CtC" );
            break;
        case PROGRAM_CHANGE:
            strcpy( ref, "PgC" );
            break;
        case CHANNEL_PRESSURE:
            strcpy( ref, "ChP" );
            break;
        case PITCH_BEND:
            strcpy( ref, "PtB" );
            break;
        default:
            strcpy( ref, "Un?" );
    };
}

void MidiMessage::SetMessageType( uint8_t messageType ) {
    
    if ( messageType < NOTE_OFF || messageType > SYSTEM_MESSAGE ) {
        return;
    } else {
        // message type is top nibble
        int8_t topNibble = messageType << 4 ;
        int8_t bottomNibble = statusByte & 0b00001111;;
        statusByte = topNibble + bottomNibble;
    }

};

const uint8_t MidiMessage::GetMessageType() {
    // message type is top four bits
    uint8_t messageType = statusByte >> 4 & 0b00001111;
    return messageType;
}

void MidiMessage::GetDataByte2TypeString( char * ref ) {
   // message type is top four bits
    uint8_t messageType = GetMessageType();
    ref[ 0 ] = '\0';
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( ref, "PITCH" );
            break;
        case NOTE_ON:
            strcpy( ref, "PITCH" );
            break;
        case KEY_PRESSURE:
            strcpy( ref, "PITCH" );
            break;
        case CONTROL_CHANGE:
            strcpy( ref, "CONTROLLER NUMBER" );
            break;
        case PROGRAM_CHANGE:
            strcpy( ref, "PROGRAM" );
            break;
        case CHANNEL_PRESSURE:
            strcpy( ref, "PRESSURE" );
            break;
        case PITCH_BEND:
            strcpy( ref, "LEAST SIGNIFICANT BIT" );
            break;
        default:
            strcpy( ref, "UNKNOWN?" );
    }
}

void MidiMessage::GetDataByte2TypeChar( char * ref ) {
    uint8_t messageType = GetMessageType();
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( ref, "p" );
            break;
        case NOTE_ON:
            strcpy( ref, "p" );
            break;
        case KEY_PRESSURE:
            strcpy( ref, "p" );
            break;
        case CONTROL_CHANGE:
            strcpy( ref, "c" );
            break;
        case PROGRAM_CHANGE:
            strcpy( ref, "p" );
            break;
        case CHANNEL_PRESSURE:
            strcpy( ref, "p" );
            break;
        case PITCH_BEND:
            strcpy( ref, "l" );
            break;
        default:
            strcpy( ref, "?" );
    };
}

void MidiMessage::GetDataByte3TypeString( char * ref ) {
   // message type is top four bits
    uint8_t messageType = GetMessageType();
    ref[ 0 ] = '\0';
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( ref, "VELOCITY" );
            break;
        case NOTE_ON:
            strcpy( ref, "VELOCITY" );
            break;
        case KEY_PRESSURE:
            strcpy( ref, "PRESSURE" );
            break;
        case CONTROL_CHANGE:
            strcpy( ref, "VALUE" );
            break;
        case PROGRAM_CHANGE:
            strcpy( ref, "(UNUSED)" );
            break;
        case CHANNEL_PRESSURE:
            strcpy( ref, "(UNUSED)" );
            break;
        case PITCH_BEND:
            strcpy( ref, "MOST SIGNIFICANT BIT" );
            break;
        default:
            strcpy( ref, "UNKNOWN?" );
    }
}

void MidiMessage::GetDataByte3TypeChar( char * ref ) {
    uint8_t messageType = GetMessageType();
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( ref, "v" );
            break;
        case NOTE_ON:
            strcpy( ref, "v" );
            break;
        case KEY_PRESSURE:
            strcpy( ref, "p" );
            break;
        case CONTROL_CHANGE:
            strcpy( ref, "v" );
            break;
        case PROGRAM_CHANGE:
            strcpy( ref, " " );
            break;
        case CHANNEL_PRESSURE:
            strcpy( ref, " " );
            break;
        case PITCH_BEND:
            strcpy( ref, "m" );
            break;
        default:
            strcpy( ref, "?" );
    };
}

const uint8_t MidiMessage::GetChannel() {
    // channel is bottom four bits
    uint8_t channelNibble = statusByte & 0b00001111;
    uint8_t channel = channelNibble + 1;
    return channel;
}

void MidiMessage::SetChannel( uint8_t channel ) {
    if ( channel < 1 || channel > 16 ) {
        return;
    } else {
        // channel is bottom nibble
        int8_t topNibble = statusByte & 0b11110000 ;
        int8_t bottomNibble = channel - 1;
        statusByte = topNibble + bottomNibble;
    }
}

void MidiMessage::Report( Logger * logger ) {
    // message type
    char buffer[ 23 ];
    GetMessageTypeString( buffer );
    logger->print( "Message Type: ");
    logger->print( buffer );

    // channel
    logger->print( ", Channel: ");
    logger->print( GetChannel() );

    // data2
    GetDataByte2TypeString( buffer );
    logger->print( ", Data2: ");
    logger->print( buffer );
    logger->print( " " );
    logger->print( dataByte2 );

    // data3
    GetDataByte3TypeString( buffer );
    logger->print( ", Data3: ");
    logger->print( buffer );
    logger->print( " " );
    logger->print( dataByte3 );
    logger->println( "" );
}

void MidiMessage::ReportLcdLine( char * ref ) {
    // message type
    char m[4];
    GetMessageTypeChar( m );

    // data2
    char d2[2];
    GetDataByte2TypeChar( d2 );
    
    if ( GetMessageType() == PROGRAM_CHANGE || GetMessageType() == CHANNEL_PRESSURE ) {
        // 2 byte messages
        // lcdLine
        snprintf( ref, 17, "%s c%02d %s%03d", m, GetChannel(), d2, dataByte2 );

    } else {
        // data3
        char d3[2];
        GetDataByte3TypeChar( d3 );
        
        if ( GetMessageType() == NOTE_OFF || GetMessageType() == NOTE_ON || GetMessageType() == KEY_PRESSURE ) {
            // pitched
            // lcdLine
            char noteName[ 4 ];
            GetNoteName( noteName );
            snprintf( ref, 17, "%s c%02d %s%s%s%03d", m, GetChannel(), d2, noteName, d3, dataByte3 );
        } else {
            //unpitched
            // lcdLine
            snprintf( ref, 17, "%s c%02d %s%03d%s%03d", m, GetChannel(), d2, dataByte2, d3, dataByte3 );
        }

    }
}

const uint8_t MidiMessage::GetDataByte2() {
    return dataByte2;
}

void MidiMessage::SetDataByte2( uint8_t d2 ) {
    dataByte2 = d2;
}

void MidiMessage::GetNoteName( char * ref ) {
    // note names 2 chars each
    const char notes[ 25 ] = "C C#D D#E F F#G G#A A#B ";

    // get octave number
    div_t divisionResult = div( dataByte2, 12 );
    char octave[ 3 ];
    octave[ 2 ] = '\0';
    snprintf( octave, 3, "%d", ( divisionResult.quot - 1 ) );

    // copy note data to ref
    memcpy( &ref[ 0 ], &notes[ ( divisionResult.rem ) * 2 ], 1 );
    memcpy( &ref[ 1 ], &notes[ ( ( divisionResult.rem ) * 2 ) + 1 ], 1 );
    memcpy( &ref[ 2 ], &octave[ 0 ], 1 );
    ref[ 3 ] = '\0';

}

const uint8_t MidiMessage::GetDataByte3() {
    return dataByte3;
}

void MidiMessage::SetDataByte3( uint8_t d3 ) {
    dataByte3 = d3;
}

void MidiMessage::byteToBinaryString(char * ref, uint8_t aByte) {
    // represent byte as 8-digit binary number
    ref[0] = '\0';
    for ( int i = 7; i >= 0; --i ) {
        ( aByte & ( 1 << i ) ) ? strcat( ref, "1" ) : strcat( ref, "0" );
    }
}

