/*

Midi Message Translation Map

*/

#ifndef __MidiMessageTranslationMap__
#define __MidiMessageTranslationMap__

#include "MidiMessage.h"

#include "MidiMessageTranslation.h"

#include <map>

class MidiMessageTranslationMap {
    private:
        std::map < uint8_t, MidiMessageTranslation > msgTranslationMap;
    public:
        MidiMessageTranslationMap(); // empty constructor
        ~MidiMessageTranslationMap();
        void InsertTranslation( MidiMessageTranslation msgT );
        const size_t GetSize();
        void Report();
        bool TranslationExists( uint8_t statusByte );
        void Translate( MidiMessage * msg );
};

#endif
