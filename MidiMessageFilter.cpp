/*

Midi Message Filter

*/

#include "MidiMessageFilter.h"

// PRIVATE

MidiMessageFilter::MidiMessageFilter() {
    // constructor

    // set all channel filters to off
    for ( uint8_t i = 1; i < 17; i++ ) {
        channelFilter[ i ] = false;
    }

    // set all message type filters to off
    for ( uint8_t i = NOTE_OFF; i <= SYSTEM_MESSAGE; i++ ) {
        messageTypeFilter[ i ] = false;
    }
};

MidiMessageFilter::~MidiMessageFilter() {
    // destructor
};

const String MidiMessageFilter::MessageTypeString( uint8_t messageType ) {
    // message type as string
    String messageTypeString;
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
        case SYSTEM_MESSAGE:
            messageTypeString = "SYSTEM MESSAGE";
            break;
        default:
            messageTypeString = "UNKNOWN?";
    }
    return messageTypeString;
}

// PUBLIC

void MidiMessageFilter::SetChannelFilter( int8_t channel, bool state ) {
    channelFilter[ channel ] = state;
};

bool MidiMessageFilter::GetChannelFilter( int8_t channel ) {
    return channelFilter[ channel ];
};

void MidiMessageFilter::SetMessageTypeFilter( int8_t messageType, bool state ) {
    messageTypeFilter[ messageType ] = state;
};

bool MidiMessageFilter::GetMessageTypeFilter( int8_t messageType ) {
    return messageTypeFilter[ messageType ];
};


void MidiMessageFilter::Report() {
    // channel filter status
    for ( uint8_t i = 1; i < 17; i++ ) {
        Serial.print( "Channel filter: Channel " );
        Serial.print( i );
        Serial.print( " Status " );
        Serial.println( ( channelFilter[ i ] ) ? "FILTER ON" : "FILTER OFF" );
    }

    // message type filter status
    for ( uint8_t i = NOTE_OFF; i <= SYSTEM_MESSAGE; i++ ) {
        Serial.print( "Message type filter: Message type " );
        Serial.print( MessageTypeString( i ) );
        Serial.print( " Status " );
        Serial.println( ( messageTypeFilter[ i ] ) ? "FILTER ON" : "FILTER OFF" );
    }

};

bool MidiMessageFilter::CheckMidiMessageForFilter( MidiMessage midiMessage ) {
    if ( channelFilter[ midiMessage.GetChannel() ] == FILTER_ON ) {
        return FILTER_ON;
    }

    if ( messageTypeFilter[ midiMessage.GetMessageType() ] == FILTER_ON ) {
        return FILTER_ON;
    }

    return FILTER_OFF;
};