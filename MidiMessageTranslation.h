/*

Midi Message Translation 

*/

#ifndef __MidiMessageTranslation__
#define __MidiMessageTranslation__

#include "MidiMessage.h"

#include <map>

class MidiMessageTranslation {
    private:
        std::pair < uint8_t, uint8_t > messageTypePair;
        std::pair < uint8_t, uint8_t > channelPair;
        std::map < uint8_t, uint8_t > dataByte2Map;
        std::map < uint8_t, uint8_t > dataByte3Map;
    public:
        MidiMessageTranslation(); // empty constructor
        MidiMessageTranslation( 
            std::pair < uint8_t, uint8_t > msgType,
            std::pair < uint8_t, uint8_t > ch
        ); // constructor
        ~MidiMessageTranslation();
        void SetMessageTypePair( uint8_t m, uint8_t mt );
        std::pair < uint8_t, uint8_t > GetMessageTypePair();
        void SetChannelPair( uint8_t c, uint8_t ct );
        std::pair < uint8_t, uint8_t > GetChannelPair();
        void Report();
        const std::string GetMessageTypeString( uint8_t msgType );
        const char * GetMessageTypeChar( uint8_t msgType );
        uint8_t GetStatusByte();
        std::map < uint8_t, uint8_t > GetDataByte2();
        void AddDataByte2MapPair( uint8_t d2, uint8_t d2t );
        const size_t GetDataByte2MapSize();
        std::pair < uint8_t, uint8_t > GetDataByte2MapPairByValue( uint8_t v );


        std::map < uint8_t, uint8_t > GetDataByte3();
        void AddDataByte3MapPair( uint8_t d3, uint8_t d3t );
        const size_t GetDataByte3MapSize();
        std::pair < uint8_t, uint8_t > GetDataByte3MapPairByValue( uint8_t v );

};

#endif
