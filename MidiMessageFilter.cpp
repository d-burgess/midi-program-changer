/*

Midi Message Filter

*/

#include "MidiMessageFilter.h"

// PRIVATE

MidiMessageFilter::MidiMessageFilter( Logger * pLogger ) {
    // constructor
    this->pLogger = pLogger;

    // set all channel filters to off
    for ( uint8_t i = 1; i < 17; i++ ) {
        channelFilter[ i ] = false;
    }

    // set all message type filters to off
    for ( uint8_t i = NOTE_OFF; i <= SYSTEM_MESSAGE; i++ ) {
        messageTypeFilter[ i ] = false;
    }
}

MidiMessageFilter::~MidiMessageFilter() {
    // destructor
}

void MidiMessageFilter::MessageTypeString( char * ref, uint8_t messageType ) {
    // message type as string
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
        case SYSTEM_MESSAGE:
            strcpy( ref, "SYSTEM MESSAGE" );
            break;
        default:
            strcpy( ref, "UNKNOWN?" );
    }
}

// PUBLIC

void MidiMessageFilter::SetChannelFilter( int8_t channel, bool state ) {
    channelFilter[ channel ] = state;
}

bool MidiMessageFilter::GetChannelFilter( int8_t channel ) {
    return channelFilter[ channel ];
}

void MidiMessageFilter::SetMessageTypeFilter( int8_t messageType, bool state ) {
    messageTypeFilter[ messageType ] = state;
}

bool MidiMessageFilter::GetMessageTypeFilter( int8_t messageType ) {
    return messageTypeFilter[ messageType ];
}

void MidiMessageFilter::Report() {
    // channel filter status
    for ( uint8_t i = 1; i < 17; i++ ) {
        pLogger->print( "Channel filter: Channel " );
        pLogger->print( i );
        pLogger->print( ": Status: " );
        pLogger->println( ( channelFilter[ i ] ) ? "FILTER ON" : "FILTER OFF" );
    }

    // message type filter status
    for ( uint8_t i = NOTE_OFF; i <= SYSTEM_MESSAGE; i++ ) {
        pLogger->print( "Message type filter: Message type " );
        char buffer[ 17 ];
        buffer[ 0 ] = '\0';
        MessageTypeString( buffer, i );
        pLogger->print( buffer );
        pLogger->print( ": Status: " );
        pLogger->println( ( messageTypeFilter[ i ] ) ? "FILTER ON" : "FILTER OFF" );
    }

}

bool MidiMessageFilter::CheckMidiMessageForFilter( MidiMessage midiMessage ) {
    if ( channelFilter[ midiMessage.GetChannel() ] == FILTER_ON ) {
        return FILTER_ON;
    }

    if ( messageTypeFilter[ midiMessage.GetMessageType() ] == FILTER_ON ) {
        return FILTER_ON;
    }

    return FILTER_OFF;
}

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
    eResult = SetFilterFromXmlNode( pChannelFilter, pChannelFilter->Value(), "channel" );
    if ( eResult != tinyxml2::XML_SUCCESS ) return eResult;
    tinyxml2::XMLNode * pMessageTypeFilter = pChannelFilter->NextSibling();
    if ( pMessageTypeFilter == nullptr ) return tinyxml2::XML_ERROR_PARSING_ELEMENT;
    eResult = SetFilterFromXmlNode( pMessageTypeFilter, pMessageTypeFilter->Value(), "messageType" );
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
        if ( strcmp( filterName, "ChannelFilter" ) == 0 ) { // zero return is equality
            SetChannelFilter( attributeNumber, status );
        } else if ( strcmp( filterName, "MessageTypeFilter" ) == 0 ) {
            SetMessageTypeFilter( attributeNumber, status );
        }
        pFilterElement = pFilterElement->NextSiblingElement( "Filter" );
    }

    return tinyxml2::XML_SUCCESS;
}