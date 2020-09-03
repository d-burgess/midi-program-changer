// MIDI Program Changer for Teensy 3.2

#include "midi-program-changer.h"

#if defined( TEENSYDUINO ) 
    //
#elif defined( UNIT_TESTING )
    usb_serial_class Serial;
#else
    usb_serial_class Serial;
    HardwareSerial Serial1;
#endif

// create logger to abstract Serial
usb_serial_class * hw = &Serial;
Logger * pLogger = new Logger( hw );

// create buffer for lcd
LiquidCrystalFast * pLcd = new LiquidCrystalFast( 2, 8, 3, 4, 5, 6, 7 );
LcdBuffer lcdBuffer( pLcd, pLogger, 20, 4 );

// initialize static member of class MidiMessage
uint8_t MidiMessage::runningStatus( 0 );

// create buffer for midi messages
MidiMessageFIFOBuffer msgBuffer;

// create filter for midi messages
MidiMessageFilter msgFilter( pLogger );

// create translation for midi messages
MidiMessageTranslation msgTranslation;

// create translation map for midi messages
MidiMessageTranslationMap msgTranslationMap;

void setup() {
    pLogger->println( "Midi Program Changer" );
    pLogger->println( "--------------------" );
    pLogger->println( "Starting setup" );

    pLogger->println( "Waiting for MIDI input serial" );
    HWSERIAL.begin( 31250 );
    while ( !HWSERIAL ) delay( 1 ); // wait until the midi input serial port has opened
    pLogger->println( "MIDI input serial established" );
    
    pLogger->println( "Waiting for communication serial" );
    while ( !Serial ) delay( 1 ); // wait until the communication serial port has opened
    pLogger->println( "Communication serial established" );

    pLogger->println( "Showing welcome message" );
    lcdBuffer.UpdateBuffer( 0, "PROGRAM CHANGER" );
    lcdBuffer.UpdateLcdFromBuffer();

    lcdBuffer.UpdateBuffer( 1, "================" );
    lcdBuffer.UpdateLcdFromBuffer();
/*
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
 */
    pLogger->println( "Setup complete" );
}

unsigned long refreshRate = 1000; // microseconds

void loop() {

    // timer for refresh rate
    static unsigned long previousMicros = 0;
    unsigned long currentMicros = micros();

    // if data waiting on serial, read a MIDI message, check message against filter and add to message buffer if not filtered 
    if ( HWSERIAL.available() > 0 ) { // data waiting on midi input serial port
        pLogger->println( "Serial available" );
        MidiMessage serialMsg( &Serial1 );
        pLogger->print( " INPUT: " );
        serialMsg.Report( pLogger );
        char lcdLine[ 17 ];
        serialMsg.ReportLcdLine( lcdLine );
        lcdBuffer.UpdateBuffer( 0, lcdLine );
        // lcdBuffer.UpdateLcdFromBuffer();
        if ( msgFilter.CheckMidiMessageForFilter( serialMsg ) == FILTER_OFF ) {
            msgBuffer.Push( serialMsg );
            pLogger->println( "PASSED" );
        } else {
            pLogger->println( "FILTERED" );
        }
    }

    // if no data waiting on serial, get MIDI message from the buffer, translate if 
    // translation exists, then output to serial
    if ( HWSERIAL.available() == 0 ) {
        if ( msgBuffer.Count() > 0 ) {
            MidiMessage bufferMsg = msgBuffer.Pop();
            if ( msgTranslationMap.TranslationExists( bufferMsg.GetStatusByte() ) ) {
                msgTranslationMap.Translate( &bufferMsg );
            }

            pLogger->print( " OUTPUT: " );
            bufferMsg.Report( pLogger );
            char lcdLine[ 17 ];
            bufferMsg.ReportLcdLine( lcdLine );
            lcdBuffer.UpdateBuffer( 1, lcdLine );

            // OUTPUT MIDI MESSAGE HERE
        }
    }

    // if no data waiting on serial and MIDI message buffer is empty, update the lcd if 
    // the refresh timer limit has been reached
    if ( HWSERIAL.available() == 0 && msgBuffer.Count() == 0 ) {
        if ( currentMicros - previousMicros >= refreshRate ) {
            previousMicros = currentMicros;
            lcdBuffer.UpdateLcdFromBuffer();
        }
    }

} // end loop
