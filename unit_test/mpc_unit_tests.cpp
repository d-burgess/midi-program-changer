#define UNIT_TESTING
   
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../fake/LiquidCrystalFast.h"
#include "../fake/Serial.h"
#include "../fake/Arduino.h"
#include "../HardwareSerial.h"
#include "../LcdBuffer.h"
#include "../MidiMessage.h"
#include "../MidiMessageFIFOBuffer.h"
#include "../MidiMessageFilter.h"
#include "../MidiMessageTranslation.h"
#include "../MidiMessageTranslationMap.h"
#include "../tinyxml2.h"
#include "midi-program-changer.h"
// #include "midi-program-changer.ino"

using ::testing::Return;

uint8_t MidiMessage::runningStatus = 0;

Serial_ Serial;

TEST( setup, pushed ) {  

    //

}
