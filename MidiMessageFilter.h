/*

Midi Message Filter

*/

#ifndef __MidiMessageFilter__
#define __MidiMessageFilter__

#include "MidiMessage.h"

#include "tinyxml2.h"
#include <string>
// #include <cassert>

#if defined( TEENSYDUINO ) 
    // #include "Arduino.h"
    #include "src/LiquidCrystalFast/LiquidCrystalFast.h"
    // #include "usb_serial.h"
#else
    #include "fake/LiquidCrystalFast.h"
    #include "fake/HardwareSerial.h"
    #include "fake/Serial.h"
    #include "fake/Arduino.h"
#endif

// #define NDEBUG

#define FILTER_ON true
#define FILTER_OFF false

class MidiMessageFilter {
    private:
        bool channelFilter[ 17 ]; // filter status for channel of midi message
        bool messageTypeFilter[ 17 ]; // filter status for message type of midi message
        void MessageTypeString( char * ref, uint8_t messageType );
        Logger * pLogger;
    public:
        MidiMessageFilter( Logger * pLogger ); // constructor
        ~MidiMessageFilter();
        void SetChannelFilter( int8_t channel, bool state );
        bool GetChannelFilter( int8_t channel );
        void SetMessageTypeFilter( int8_t messageType, bool state );
        bool GetMessageTypeFilter( int8_t messageType );
        void Report();
        bool CheckMidiMessageForFilter( MidiMessage midiMessage );



        std::string GetFilterAsXmlString(); 
        void AddFilterXmlElement( const char * type, uint8_t i, tinyxml2::XMLNode * pChannelFilter, tinyxml2::XMLDocument * xmlDoc );
        void AddFilterXmlNode( const char * filterName, const char * type, uint8_t startIndex, tinyxml2::XMLNode * pRoot, tinyxml2::XMLDocument * xmlDoc );
        std::string GetXmlObjectAsStdString( tinyxml2::XMLDocument * xmlDoc );

        uint8_t SetFilterFromXmlString( std::string xmlString );
        tinyxml2::XMLError SetFilterFromXmlNode( tinyxml2::XMLNode * pFilter, const char * filterName, const char * attribute  );

};

#endif
