/*

Midi Message Filter

*/

#include "MidiMessageFilter.h"

#ifndef TEENSYDUINO
    extern Serial_ Serial;
#endif

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

const std::string MidiMessageFilter::MessageTypeString( uint8_t messageType ) {
    // message type as string
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
        // Serial.print( "Channel filter: Channel " );
        // Serial.print( i );
        // Serial.print( " Status " );
        // Serial.println( ( channelFilter[ i ] ) ? "FILTER ON" : "FILTER OFF" );
    }

    // message type filter status
    for ( uint8_t i = NOTE_OFF; i <= SYSTEM_MESSAGE; i++ ) {
        // Serial.print( "Message type filter: Message type " );
        // Serial.print( MessageTypeString( i ) );
        // Serial.print( " Status " );
        // Serial.println( ( messageTypeFilter[ i ] ) ? "FILTER ON" : "FILTER OFF" );
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

std::string MidiMessageFilter::GetFilterAsXmlString() {
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLNode * pRoot = xmlDoc.NewElement( "MidiMessageFilter" );
    xmlDoc.InsertEndChild( pRoot );
    AddFilterXmlNode( "ChannelFilter", "channel", 1, pRoot, &xmlDoc );
    AddFilterXmlNode( "MessageTypeFilter", "messageType", 8, pRoot, &xmlDoc );
    std::string xmlString = GetXmlObjectAsStdString( &xmlDoc );
    return xmlString;
}

void MidiMessageFilter::AddFilterXmlNode( const char * filterName, const char * type, uint8_t startIndex, tinyxml2::XMLNode * pRoot, tinyxml2::XMLDocument * xmlDoc ) {
    tinyxml2::XMLNode * pChannelFilter = xmlDoc->NewElement( filterName );
    pRoot->InsertEndChild ( pChannelFilter );
    for ( size_t i = startIndex; i < 17; ++i ) {
        AddFilterXmlElement( type, i, pChannelFilter, xmlDoc );
    }
}

void MidiMessageFilter::AddFilterXmlElement( const char * type, uint8_t i, tinyxml2::XMLNode * pChannelFilter, tinyxml2::XMLDocument * xmlDoc ) {
    tinyxml2::XMLElement * pFilterElement = xmlDoc->NewElement( "Filter" );
    pFilterElement->SetAttribute( type, i );
    pFilterElement->SetAttribute( "status", GetChannelFilter( i ) );
    pChannelFilter->InsertEndChild(pFilterElement);
}

std::string MidiMessageFilter::GetXmlObjectAsStdString( tinyxml2::XMLDocument * xmlDoc ) {
    tinyxml2::XMLPrinter myPrinter( 0, true, 0 );
    xmlDoc->Print( &myPrinter );
    std::string xmlString( myPrinter.CStr() );
    return xmlString;
}

uint8_t MidiMessageFilter::SetFilterFromXmlString( std::string xmlString ) {
    const char * cstr = xmlString.c_str();
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError eResult = xmlDoc.Parse( cstr );
    if ( eResult != tinyxml2::XML_SUCCESS ) return eResult;

    tinyxml2::XMLNode * pRoot = xmlDoc.FirstChild();
    if ( pRoot == nullptr ) return tinyxml2::XML_ERROR_PARSING_ELEMENT;
 
    tinyxml2::XMLNode * pChannelFilter = pRoot->FirstChild();
    if ( pChannelFilter == nullptr ) return tinyxml2::XML_ERROR_PARSING_ELEMENT;

    eResult = SetFilterFromXmlNode( pChannelFilter, "ChannelFilter", "channel" );
    if ( eResult != tinyxml2::XML_SUCCESS ) return eResult;

    tinyxml2::XMLNode * pMessageTypeFilter = pChannelFilter->NextSibling();
    if ( pMessageTypeFilter == nullptr ) return tinyxml2::XML_ERROR_PARSING_ELEMENT;

    eResult = SetFilterFromXmlNode( pMessageTypeFilter, "MessageTypeFilter", "messageType" );
    if ( eResult != tinyxml2::XML_SUCCESS ) return eResult;

    return tinyxml2::XML_SUCCESS;
}

tinyxml2::XMLError MidiMessageFilter::SetFilterFromXmlNode( tinyxml2::XMLNode * pFilter, const char * filterName, const char * attribute ) {
    tinyxml2::XMLElement * pFilterElement = pFilter->FirstChildElement();
    if ( pFilterElement == nullptr ) return tinyxml2::XML_ERROR_PARSING_ELEMENT;
    
    int attributeNumber;
    bool status;
    tinyxml2::XMLError eResult;
    while ( pFilterElement != nullptr ) {
        eResult = pFilterElement->QueryIntAttribute( attribute, &attributeNumber );
        if ( eResult != tinyxml2::XML_SUCCESS ) return eResult;
        eResult = pFilterElement->QueryBoolAttribute( "status", &status );
        if ( eResult != tinyxml2::XML_SUCCESS ) return eResult;
        if ( filterName == "ChannelFilter" ) {
            SetChannelFilter( attributeNumber, status );
        } else if ( filterName == "MessageTypeFilter" ) {
            SetMessageTypeFilter( attributeNumber, status );
        }
        pFilterElement = pFilterElement->NextSiblingElement( "Filter" );
    }

    return tinyxml2::XML_SUCCESS;
}