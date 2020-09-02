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

MidiMessage::MidiMessage( bool readFromSerial ) {
    // constructor to get midi message from serial
    bool thirdByteFlag = false;
    bool messageComplete = false;

    do {
        if ( HWSERIAL.available() > 0 ) { 
            uint8_t serialByte = HWSERIAL.read(); // read a byte from serial
            //  if ( serialByte != 0b11111110 ) { // active sensing
            //     // output 8-digit binary number to serial
            //     char binary[ 9 ];
            //     binary[ 0 ] = '\0';
            //     for ( int i = 7; i >= 0; --i ) {
            //         ( serialByte & ( 1 << i ) ) ? strcat( binary, "1" ) : strcat( binary, "0" );
            //     }
            //     Serial.print( binary );
            // }
            if ( serialByte & STATUS_BIT ) { // Status byte received
                runningStatus = serialByte;
                thirdByteFlag = false;
                // Serial.print( " Status byte " );
                // Serial.print( GetMessageTypeString() );
                // Serial.print( " running status now " );
                // Serial.println( runningStatus );
            } else {
                if ( thirdByteFlag ) { // Second data byte received
                    thirdByteFlag = false;
                    dataByte3 = serialByte;
                    // Serial.println( " data byte 3 complete" );
                    messageComplete = true;
                } else { // First data byte received
                    if ( !runningStatus ) { // no status byte
                        // ignore invalid data byte
                        // Serial.println( " no running status - ignored " );
                    } else {
                        // running status is valid
                        if ( runningStatus < ( PROGRAM_CHANGE << 4 ) ) { // First data byte of Note Off/On, Key Pressure or Control Change
                            thirdByteFlag = true;
                            statusByte = runningStatus;
                            dataByte2 = serialByte;
                            // Serial.print( " data byte 2 incomplete < PgC " );
                            // Serial.print( runningStatus );
                            // Serial.print( " < " );
                            // Serial.println( PROGRAM_CHANGE << 4 );
                        } else if ( runningStatus < ( PITCH_BEND << 4 ) ) { // First data byte of Program Change or Channel Pressure
                            statusByte = runningStatus;
                            dataByte2 = serialByte;
                            dataByte3 = 0;
                            // Serial.print( " data byte 2 complete < PtB " );
                            // Serial.print( runningStatus );
                            // Serial.print( " < " );
                            // Serial.println( PITCH_BEND << 4 );
                            messageComplete = true;
                        } else if ( runningStatus < 0xF0 ) { // First data byte of Pitch Bend
                            thirdByteFlag = true;
                            statusByte = runningStatus;
                            dataByte2 = serialByte;
                            // Serial.print( " data byte 2 incomplete < 0xF0 " );
                            // Serial.print( runningStatus );
                            // Serial.print( " < " );
                            // Serial.println( 0xF0 );
                        } else { // System message, ignore
                            runningStatus = 0;
                            // Serial.println( " System message - ignored" );
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

const std::string MidiMessage::GetMessageTypeString() {
    // message type is top four bits
    uint8_t messageType = GetMessageType();
    std::string messageTypeString;
    switch ( messageType ) {
        case NOTE_OFF:
            messageTypeString = "NOTE OFF";
            break;
        case NOTE_ON:
            messageTypeString = "NOTE ON";
            break;
        case KEY_PRESSURE:
            messageTypeString = "KEY PRESSURE";
            break;
        case CONTROL_CHANGE:
            messageTypeString = "CONTROL CHANGE";
            break;
        case PROGRAM_CHANGE:
            messageTypeString = "PROGRAM CHANGE";
            break;
        case CHANNEL_PRESSURE:
            messageTypeString = "CHANNEL PRESSURE";
            break;
        case PITCH_BEND:
            messageTypeString = "PITCH BEND";
            break;
        default:
            messageTypeString = "UNKNOWN?";
    }
    return messageTypeString;
}

void MidiMessage::GetMessageTypeChar( char * reference ) {
    uint8_t messageType = GetMessageType();
    std::string dataByte2Type;
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( reference, "NOf" );
            break;
        case NOTE_ON:
            strcpy( reference, "NOn" );
            break;
        case KEY_PRESSURE:
            strcpy( reference, "KyP" );
            break;
        case CONTROL_CHANGE:
            strcpy( reference, "CtC" );
            break;
        case PROGRAM_CHANGE:
            strcpy( reference, "PgC" );
            break;
        case CHANNEL_PRESSURE:
            strcpy( reference, "ChP" );
            break;
        case PITCH_BEND:
            strcpy( reference, "PtB" );
            break;
        default:
            strcpy( reference, "Un?" );
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

const std::string MidiMessage::GetDataByte2TypeString() {
   // message type is top four bits
    uint8_t messageType = GetMessageType();
    std::string dataByte2Type;
    switch ( messageType ) {
        case NOTE_OFF:
            dataByte2Type = "PITCH";
            break;
        case NOTE_ON:
            dataByte2Type = "PITCH";
            break;
        case KEY_PRESSURE:
            dataByte2Type = "PITCH";
            break;
        case CONTROL_CHANGE:
            dataByte2Type = "CONTROLLER NUMBER";
            break;
        case PROGRAM_CHANGE:
            dataByte2Type = "PROGRAM";
            break;
        case CHANNEL_PRESSURE:
            dataByte2Type = "PRESSURE";
            break;
        case PITCH_BEND:
            dataByte2Type = "LEAST SIGNIFICANT BIT";
            break;
        default:
            dataByte2Type = "UNKNOWN?";
    }
    return dataByte2Type;
}

void MidiMessage::GetDataByte2TypeChar( char * reference ) {
    uint8_t messageType = GetMessageType();
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( reference, "p" );
            break;
        case NOTE_ON:
            strcpy( reference, "p" );
            break;
        case KEY_PRESSURE:
            strcpy( reference, "p" );
            break;
        case CONTROL_CHANGE:
            strcpy( reference, "c" );
            break;
        case PROGRAM_CHANGE:
            strcpy( reference, "p" );
            break;
        case CHANNEL_PRESSURE:
            strcpy( reference, "p" );
            break;
        case PITCH_BEND:
            strcpy( reference, "l" );
            break;
        default:
            strcpy( reference, "?" );
    };
}

const std::string MidiMessage::GetDataByte3TypeString() {
   // message type is top four bits
    uint8_t messageType = GetMessageType();
    std::string dataByte3Type;
    switch ( messageType ) {
        case NOTE_OFF:
            dataByte3Type = "VELOCITY";
            break;
        case NOTE_ON:
            dataByte3Type = "VELOCITY";
            break;
        case KEY_PRESSURE:
            dataByte3Type = "PRESSURE";
            break;
        case CONTROL_CHANGE:
            dataByte3Type = "VALUE";
            break;
        case PROGRAM_CHANGE:
            dataByte3Type = "(UNUSED)";
            break;
        case CHANNEL_PRESSURE:
            dataByte3Type = "(UNUSED)";
            break;
        case PITCH_BEND:
            dataByte3Type = "MOST SIGNIFICANT BIT";
            break;
        default:
            dataByte3Type = "UNKNOWN?";
    }
    return dataByte3Type;
}

void MidiMessage::GetDataByte3TypeChar( char * reference ) {
    uint8_t messageType = GetMessageType();
    switch ( messageType ) {
        case NOTE_OFF:
            strcpy( reference, "v" );
            break;
        case NOTE_ON:
            strcpy( reference, "v" );
            break;
        case KEY_PRESSURE:
            strcpy( reference, "p" );
            break;
        case CONTROL_CHANGE:
            strcpy( reference, "v" );
            break;
        case PROGRAM_CHANGE:
            strcpy( reference, " " );
            break;
        case CHANNEL_PRESSURE:
            strcpy( reference, " " );
            break;
        case PITCH_BEND:
            strcpy( reference, "m" );
            break;
        default:
            strcpy( reference, "?" );
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

void MidiMessage::Report() {
    // message type
    std::string m = GetMessageTypeString();
    Serial.print( "Message Type: ");
    Serial.print( m );

    // channel
    Serial.print( ", Channel: ");
    Serial.print( GetChannel() );

    // data2
    std::string d2 = GetDataByte2TypeString();
    Serial.print( ", Data2: ");
    Serial.print( d2 );
    Serial.print( " " );
    Serial.print( dataByte2 );

    // data3
    std::string d3 = GetDataByte3TypeString();
    Serial.print( ", Data3: ");
    Serial.print( d3 );
    Serial.print( " " );
    Serial.println( dataByte3 );
}

void MidiMessage::ReportLcdLine( char * reference ) {
    // message type
    char m[4];
    GetMessageTypeChar( m );

    // data2
    char d2[2];
    GetDataByte2TypeChar( d2 );
    
    if ( GetMessageType() == PROGRAM_CHANGE || GetMessageType() == CHANNEL_PRESSURE ) {
        // 2 byte messages
        // lcdLine
        snprintf( reference, 17, "%s c%02d %s%03d", m, GetChannel(), d2, dataByte2 );

    } else {
        // data3
        char d3[2];
        GetDataByte3TypeChar( d3 );
        
        if ( GetMessageType() == NOTE_OFF || GetMessageType() == NOTE_ON || GetMessageType() == KEY_PRESSURE ) {
            // pitched
            // lcdLine
            char noteName[ 4 ];
            GetNoteName( noteName );
            snprintf( reference, 17, "%s c%02d %s%s%s%03d", m, GetChannel(), d2, noteName, d3, dataByte3 );
        } else {
            //unpitched
            // lcdLine
            snprintf( reference, 17, "%s c%02d %s%03d%s%03d", m, GetChannel(), d2, dataByte2, d3, dataByte3 );
        }

    }
}

const uint8_t MidiMessage::GetDataByte2() {
    return dataByte2;
}

void MidiMessage::SetDataByte2( uint8_t d2 ) {
    dataByte2 = d2;
}

void MidiMessage::GetNoteName( char * reference ) {
    // note names 2 chars each
    const char notes[ 25 ] = "C C#D D#E F F#G G#A A#B ";

    // get octave number
    div_t divisionResult = div( dataByte2, 12 );
    char octave[ 3 ];
    octave[ 2 ] = '\0';
    snprintf( octave, 3, "%d", ( divisionResult.quot - 1 ) );

    // copy note data to reference
    memcpy( &reference[ 0 ], &notes[ ( divisionResult.rem ) * 2 ], 1 );
    memcpy( &reference[ 1 ], &notes[ ( ( divisionResult.rem ) * 2 ) + 1 ], 1 );
    memcpy( &reference[ 2 ], &octave[ 0 ], 1 );
    reference[ 3 ] = '\0';

}

const uint8_t MidiMessage::GetDataByte3() {
    return dataByte3;
}

void MidiMessage::SetDataByte3( uint8_t d3 ) {
    dataByte3 = d3;
}

