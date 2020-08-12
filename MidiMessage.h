/*

Midi Message

*/

#ifndef __MidiMessage__
#define __MidiMessage__

#include "Arduino.h"

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
    public:
        MidiMessage(); // empty constructor
        MidiMessage( bool readFromSerial ); // constructor from serial port data
        MidiMessage( uint8_t s, uint8_t d1, uint8_t d2 ); // manual contructor
        ~MidiMessage();
        static uint8_t runningStatus;   /* running status for storing omitted first byte
                                        of repeated similar midi messages */
        const uint8_t GetStatusByte();
        void SetMessageType( uint8_t messageType );
        const uint8_t GetMessageType();
        const String GetMessageTypeString();
        void GetMessageTypeChar( char * reference );
        const String GetDataByte2TypeString();
        void GetDataByte2TypeChar( char * reference );
        const String GetDataByte3TypeString();
        void GetDataByte3TypeChar( char * reference );
        void SetChannel( uint8_t channel );
        const uint8_t GetChannel();
        void Report();
        void ReportLcdLine( char * reference );
        const uint8_t GetDataByte2();
        void SetDataByte2( uint8_t d2 );
        void GetNoteName( char * reference );
};

#endif
