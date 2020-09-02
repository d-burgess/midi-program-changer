#ifndef __MidiProgramChanger__
#define __MidiProgramChanger__

    #ifdef UNIT_TESTING
        #include "fake/LiquidCrystalFast.h"
        #include "fake/Serial.h"
        #include "fake/Arduino.h"
    #else
        // #include "Arduino.h"
        // #include "LiquidCrystalFast.h"
    #endif

    #include "HardwareSerial.h"
    #include "LcdBuffer.h"
    #include "MidiMessage.h"
    #include "MidiMessageFIFOBuffer.h"
    #include "MidiMessageFilter.h"
    #include "MidiMessageTranslation.h"
    #include "MidiMessageTranslationMap.h"

#endif

