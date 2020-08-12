/*

Midi Message Translation Map

*/

#include "MidiMessageTranslationMap.h"

// PUBLIC

MidiMessageTranslationMap::MidiMessageTranslationMap() {
    // empty constructor
};

MidiMessageTranslationMap::~MidiMessageTranslationMap() {
    // destructor
};

void MidiMessageTranslationMap::InsertTranslation( MidiMessageTranslation msgT ) {
    msgTranslationMap.insert( std::make_pair( msgT.GetStatusByte(), msgT ) );
};

const size_t MidiMessageTranslationMap::GetSize() {
    return msgTranslationMap.size();
};

void MidiMessageTranslationMap::Report() {
    Serial.print( "Translations in map: " );
    Serial.println( msgTranslationMap.size() );
    std::map < uint8_t, MidiMessageTranslation >::iterator msgT = msgTranslationMap.begin();
    while ( msgT != msgTranslationMap.end() ) {
        msgT->second.Report();
        msgT++;
    }
};

bool MidiMessageTranslationMap::TranslationExists( uint8_t statusByte ) {
    return ( msgTranslationMap.find( statusByte ) != msgTranslationMap.end() ) ? true : false;
};

void MidiMessageTranslationMap::Translate( MidiMessage * msg ) {
    std::map < uint8_t, MidiMessageTranslation >::iterator msgT = msgTranslationMap.find( msg->GetStatusByte() );
    uint8_t newMsgType = msgT->second.GetMessageTypePair().second;
    msg->SetMessageType( newMsgType );
    uint8_t newChannel = msgT->second.GetChannelPair().second;
    msg->SetChannel( newChannel );

    // data byte translations
    if ( msgT->second.GetDataByte2MapSize() > 0 ) {
        std::pair < uint8_t, uint8_t > tp = msgT->second.GetDataByte2MapPairByValue( msg->GetDataByte2() );
        if ( tp.first != 0 && tp.second != 0 ) {
            uint8_t newDataByte2 = tp.second;
            msg->SetDataByte2( newDataByte2 );
        }
    }
};