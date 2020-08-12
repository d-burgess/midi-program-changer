/*

Midi Message Translation 

*/

#include "MidiMessageTranslation.h"

// PUBLIC

MidiMessageTranslation::MidiMessageTranslation() {
    // empty constructor
};

MidiMessageTranslation::MidiMessageTranslation(
    std::pair < uint8_t, uint8_t > msgType,
    std::pair < uint8_t, uint8_t > ch
) {
    // constructor
    messageTypePair = msgType;
    channelPair = ch;
};

MidiMessageTranslation::~MidiMessageTranslation() {
    // destructor
};

void MidiMessageTranslation::SetMessageTypePair( uint8_t m, uint8_t mt ) {
    messageTypePair = std::make_pair( m, mt );
};

std::pair < uint8_t, uint8_t > MidiMessageTranslation::GetMessageTypePair() {
    return messageTypePair;
};

void MidiMessageTranslation::SetChannelPair( uint8_t c, uint8_t ct ) {
    channelPair = std::make_pair( c, ct );
};

std::pair < uint8_t, uint8_t > MidiMessageTranslation::GetChannelPair() {
    return channelPair;
};

void MidiMessageTranslation::Report() {
    // translation source
    Serial.print( "TRANSLATION: " );
    Serial.print( GetStatusByte() );
    Serial.print( " " );
    Serial.print( GetMessageTypeString( messageTypePair.first ) );
    Serial.print( ", Channel " );
    Serial.print( channelPair.first );
    
    Serial.print( " -> " );
    
    // translation destination
    Serial.print( GetMessageTypeString( messageTypePair.second ) );
    Serial.print( ", Channel " );
    Serial.println( channelPair.second );

    // data byte translations
    if ( dataByte2Map.size() > 0 ) {
        Serial.print( "    DataByte2: " );
        std::map < uint8_t, uint8_t >::iterator it = dataByte2Map.begin();
        while ( it != dataByte2Map.end() ) {
            Serial.print( it->first );
            Serial.print( " -> " );
            Serial.println( it->second );
            it++;
        }
    }

    if ( dataByte3Map.size() > 0 ) {
        Serial.print( "    DataByte3: " );
        std::map < uint8_t, uint8_t >::iterator it = dataByte3Map.begin();
        while ( it != dataByte3Map.end() ) {
            Serial.print( it->first );
            Serial.print( " -> " );
            Serial.println( it->second );
            it++;
        }
    }
};

const String MidiMessageTranslation::GetMessageTypeString( uint8_t msgType ) {
    String messageTypeString;
    switch ( msgType ) {
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

uint8_t MidiMessageTranslation::GetStatusByte() {
    int8_t topNibble = messageTypePair.first << 4 ;
    return ( topNibble + channelPair.first - 1 );
}

std::map < uint8_t, uint8_t > MidiMessageTranslation::GetDataByte2() {
    return dataByte2Map;
};

void MidiMessageTranslation::AddDataByte2MapPair( uint8_t d2, uint8_t d2t ) {
    dataByte2Map.insert( std::make_pair( d2, d2t ) );
}

const size_t MidiMessageTranslation::GetDataByte2MapSize() {
    return dataByte2Map.size();
};

std::pair < u_int8_t, u_int8_t > MidiMessageTranslation::GetDataByte2MapPairByValue( uint8_t v ) {
        if ( dataByte2Map.find( v ) != dataByte2Map.end() ) {
            std::map < uint8_t, uint8_t >::iterator it = dataByte2Map.find( v );
            return *it;
        } else {
            return std::make_pair( 0, 0 );
        }
};