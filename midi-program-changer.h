#ifndef __MidiProgramChanger__
#define __MidiProgramChanger__

    #if defined( TEENSYDUINO ) 
        // #include "Arduino.h"
        #include "src/LiquidCrystalFast/LiquidCrystalFast.h"
        // #include "usb_serial.h"
    #else
        #include "fake/LiquidCrystalFast.h"
        #include "fake/HardwareSerial.h"
        #include "fake/usb_serial_class.h"
        #include "fake/Serial.h"
        #include "fake/Arduino.h"
    #endif

    #include "HardwareSerial.h"
    #include "LcdBuffer.h"
    #include "Logger.h"
    #include "Gui.h"
    #include "GuiElement.h"
    #include "ClickButton.h"
    #include "ButtonEventBuffer.h"
    #include "MidiMessage.h"
    #include "MidiMessageFIFOBuffer.h"
    #include "MidiMessageFilter.h"
    #include "MidiMessageTranslation.h"
    #include "MidiMessageTranslationMap.h"

#endif