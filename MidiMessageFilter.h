/*

Midi Message Filter

*/

#ifndef __MidiMessageFilter__
#define __MidiMessageFilter__

#include "MidiMessage.h"

#include "Arduino.h"

#define FILTER_ON true
#define FILTER_OFF false

class MidiMessageFilter {
    private:
        bool channelFilter[ 17 ]; // filter status for channel of midi message
        bool messageTypeFilter[ 17 ]; // filter status for message type of midi message
        const String MessageTypeString( uint8_t messageType );
    public:
        MidiMessageFilter(); // empty constructor
        ~MidiMessageFilter();
        void SetChannelFilter( int8_t channel, bool state );
        bool GetChannelFilter( int8_t channel );
        void SetMessageTypeFilter( int8_t messageType, bool state );
        bool GetMessageTypeFilter( int8_t messageType );
        void Report();
        bool CheckMidiMessageForFilter( MidiMessage midiMessage );
};

#endif
