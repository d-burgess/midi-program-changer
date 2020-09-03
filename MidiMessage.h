/*

Midi Message

*/

#ifndef __MidiMessage__
#define __MidiMessage__

#ifndef TEENSYDUINO
    #include "fake/LiquidCrystalFast.h"
    #include "fake/HardwareSerial.h"
    #include "fake/Serial.h"
    #include "fake/Arduino.h"
#else
    // #include "Arduino.h"
    #include "src/LiquidCrystalFast/LiquidCrystalFast.h" // Import the LCD Fast library
#endif

#include <stdint.h>
#include <string>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "Logger.h"

#define NOTE_OFF 0x8
#define NOTE_ON 0x9
#define KEY_PRESSURE 0xA
#define CONTROL_CHANGE 0xB
#define PROGRAM_CHANGE 0xC
#define CHANNEL_PRESSURE 0xD
#define PITCH_BEND 0xE
#define SYSTEM_MESSAGE 0xF
#define STATUS_BIT 0b10000000

#define READ_FROM_SERIAL true

class MidiMessage {
    private:
        uint8_t statusByte; // first byte of midi message
        uint8_t dataByte2; // second byte if midi message
        uint8_t dataByte3; // third byte of midi message (optional for some message types)
        void byteToBinaryString( char * ref, uint8_t aByte );
    public:
        MidiMessage(); // empty constructor
        MidiMessage( HardwareSerial * pSerial ); // constructor from serial port data
        MidiMessage( uint8_t s, uint8_t d1, uint8_t d2 ); // manual contructor
        ~MidiMessage();
        static uint8_t runningStatus;   /* running status for storing omitted first byte
                                        of repeated similar midi messages */
        const uint8_t GetStatusByte();
        void SetMessageType( uint8_t messageType );
        const uint8_t GetMessageType();
        void GetMessageTypeString( char * ref );
        void GetMessageTypeChar( char * ref );
        void GetDataByte2TypeString( char * ref );
        void GetDataByte2TypeChar( char * ref );
        void GetDataByte3TypeString( char * ref );
        void GetDataByte3TypeChar( char * ref );
        void SetChannel( uint8_t channel );
        const uint8_t GetChannel();
        void Report( Logger * logger );
        void ReportLcdLine( char * ref );
        const uint8_t GetDataByte2();
        void SetDataByte2( uint8_t d2 );
        void GetNoteName( char * ref );
        const uint8_t GetDataByte3();
        void SetDataByte3( uint8_t d3 );


};

#endif
