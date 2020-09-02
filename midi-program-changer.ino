// MIDI Program Changer for Teensy 3.2/Arduino Uno

#include "midi-program-changer.h"

// create buffer for lcd
LiquidCrystalFast lcd( 2, 8, 3, 4, 5, 6, 7 );
LcdBuffer lcdBuffer( &lcd, 16, 2 );

// initialize static member of class MidiMessage
uint8_t MidiMessage::runningStatus = 0;

// create buffer for midi messages
MidiMessageFIFOBuffer msgBuffer;

// create filter for midi messages
MidiMessageFilter msgFilter;

// create translation for midi messages
MidiMessageTranslation msgTranslation;

// create translation map for midi messages
MidiMessageTranslationMap msgTranslationMap;



void setup() {

    HWSERIAL.begin( 31250 );
    while ( !HWSERIAL ) delay( 1 ); // wait until the midi input serial port has opened

    while ( !Serial ) delay( 1 ); // wait until the communication serial port has opened
    
    lcdBuffer.UpdateBuffer( 0, "PROGRAM CHANGER" );
    lcdBuffer.UpdateLcdFromBuffer();

    lcdBuffer.UpdateBuffer( 1, "================" );
    lcdBuffer.UpdateLcdFromBuffer();

    // Create filters on INPUT
    msgFilter.SetChannelFilter( 2, FILTER_ON ); // no messages from channel 2
    msgFilter.SetMessageTypeFilter( PITCH_BEND, FILTER_ON ); // no pitch bend
    
    // Report all filters
    msgFilter.Report();

    // Create translations on OUTPUT
    // Type and channel
    msgTranslation.SetMessageTypePair( NOTE_ON, KEY_PRESSURE );
    msgTranslation.SetChannelPair( 1, 2 );
    msgTranslationMap.InsertTranslation( msgTranslation );

    // Data value
    msgTranslation.SetMessageTypePair( CONTROL_CHANGE, CONTROL_CHANGE );
    msgTranslation.SetChannelPair( 1, 1 );
    msgTranslation.AddDataByte2MapPair( 7, 10 );
    msgTranslationMap.InsertTranslation( msgTranslation );

    // Multiple data values
    msgTranslation.SetMessageTypePair( PROGRAM_CHANGE, PROGRAM_CHANGE );
    msgTranslation.SetChannelPair( 1, 1 );
    msgTranslation.AddDataByte2MapPair( 10, 11 );
    msgTranslation.AddDataByte2MapPair( 15, 17 );
    msgTranslation.AddDataByte2MapPair( 20, 23 );
    msgTranslationMap.InsertTranslation( msgTranslation );

    // Report all translations
    msgTranslationMap.Report();

}

unsigned long previousMicros = 0;
unsigned long refreshRate = 1000; // microseconds

void loop() {

    // timer for refresh rate
    unsigned long currentMicros = micros();

    // if data waiting on serial, read a MIDI message, check message against filter and add to message buffer if not filtered 
    if ( HWSERIAL.available() > 0 ) { // data waiting on midi input serial port
        unsigned long startMicros = micros();
        // Serial.print( micros() );
        MidiMessage serialMsg( READ_FROM_SERIAL );
        // Serial.print( " INPUT: " );
        serialMsg.Report();
        char lcdLine[ 17 ];
        serialMsg.ReportLcdLine( lcdLine );
        lcdBuffer.UpdateBuffer( 0, lcdLine );
        // lcdBuffer.UpdateLcdFromBuffer();
        if ( msgFilter.CheckMidiMessageForFilter( serialMsg ) == FILTER_OFF ) {
            msgBuffer.Push( serialMsg );
            // Serial.println( "PASSED" );
        } else {
            // Serial.println( "FILTERED" );
        }
        unsigned long endMicros = micros();
        // Serial.println( endMicros - startMicros );
    }

    // if no data waiting on serial, get MIDI message from the buffer, translate if 
    // translation exists, then output to serial
    if ( HWSERIAL.available() == 0 ) {
        if ( msgBuffer.Count() > 0 ) {
            unsigned long startMicros = micros();
            // Serial.print( micros() );
            MidiMessage bufferMsg = msgBuffer.Pop();
            if ( msgTranslationMap.TranslationExists( bufferMsg.GetStatusByte() ) ) {
                msgTranslationMap.Translate( &bufferMsg );
            }

            // Serial.print( " OUTPUT: " );
            bufferMsg.Report();
            char lcdLine[ 17 ];
            bufferMsg.ReportLcdLine( lcdLine );
            lcdBuffer.UpdateBuffer( 1, lcdLine );
            // lcdBuffer.UpdateLcdFromBuffer();
            // OUTPUT MIDI MESSAGE HERE
            unsigned long endMicros = micros();
            // Serial.println( endMicros - startMicros );
        }
    }

    // if no data waiting on serial and MIDI message buffer is empty, update the lcd if 
    // the refresh timer limit has been reached
    if ( HWSERIAL.available() == 0 && msgBuffer.Count() == 0 ) {
        if ( currentMicros - previousMicros >= refreshRate ) {
            unsigned long startMicros = micros();
            // Serial.print( micros() );
            // Serial.print( " LCD UPDATE: Refresh: " );
            // Serial.print( currentMicros - previousMicros );
            previousMicros = currentMicros;
            lcdBuffer.UpdateLcdFromBuffer();
            unsigned long endMicros = micros();
            // Serial.print( " Update time: " );
            // Serial.println( endMicros - startMicros );
        }
    }

} // end loop
