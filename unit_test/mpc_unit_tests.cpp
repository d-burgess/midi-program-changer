#define UNIT_TESTING
   
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../fake/LiquidCrystalFast.h"
#include "../fake/Serial.h"
#include "../fake/Arduino.h"
#include "../fake/HardwareSerial.h"
#include "../fake/usb_serial_class.h"
#include "../Logger.h"
#include "../LcdBuffer.h"
#include "../MidiMessage.h"
#include "../MidiMessageFIFOBuffer.h"
#include "../MidiMessageFilter.h"
#include "../MidiMessageTranslation.h"
#include "../MidiMessageTranslationMap.h"
#include "../tinyxml2.h"
#include "midi-program-changer.h"

class MockHardwareSerial : public HardwareSerial {
    public:
        MOCK_METHOD1( begin, void( int c ) );
        MOCK_METHOD0( available, int() );
        MOCK_METHOD0( read, uint8_t() );
};

class MockLiquidCrystalFast : public LiquidCrystalFast {
    public:
        MOCK_METHOD2( begin, void( int c, int r ) );
        MOCK_METHOD2( setCursor, void( int x, int y ) );
        MOCK_METHOD0( clear, void() );
        MOCK_METHOD1( print, void( char c ) );
};

MockLiquidCrystalFast mockLcd;

MockHardwareSerial Serial1;

#include "midi-program-changer.ino"

using ::testing::_;
using ::testing::Return;
using ::testing::Matcher;
using ::testing::AtLeast;
using ::testing::Invoke;
// using ::testing::ElementsAre; // vectors
using ::testing::Pair;

// uint8_t MidiMessage::runningStatus = 0;

// Test Fixtures

//Lcd Buffer
struct lcdBufferT : public testing::Test {
    void SetUp() { 
        std::cout<<"SetUp"<<std::endl;

    }
    void TearDown() { 
        std::cout<<"TearDown"<<std::endl;
    }
};

// Midi Message Translation Map
struct midiMessageTranslationMapT : public testing::Test {
    MidiMessageTranslationMap * mmtm;
    void SetUp() { 
        std::cout<<"SetUp"<<std::endl;
        mmtm = new MidiMessageTranslationMap; 
    }
    void TearDown() { 
        std::cout<<"TearDown"<<std::endl;
        delete mmtm; 
    }
};

// ino file tests

TEST( ino, testSetupFunction ) {  

    EXPECT_CALL( Serial1, begin( 31250 ) );
    
    setup();

}

TEST( ino, testLoopSerialRead ) {  

    EXPECT_CALL( Serial1, available() )
    .WillOnce( Return( 13 ) )
    .WillOnce( Return( 12 ) )
    .WillOnce( Return( 11 ) )
    .WillOnce( Return( 10 ) )
    .WillOnce( Return( 9 ) )
    .WillOnce( Return( 8 ) )
    .WillOnce( Return( 7 ) )
    .WillOnce( Return( 6 ) )
    .WillOnce( Return( 5 ) )
    .WillOnce( Return( 4 ) )
    .WillOnce( Return( 3 ) )
    .WillOnce( Return( 2 ) )
    .WillOnce( Return( 1 ) )
    .WillRepeatedly(Return( 0 ) );

    EXPECT_CALL( Serial1, read() )
    .WillOnce( Return( 144 ) )
    .WillOnce( Return( 50 ) )
    .WillOnce( Return( 34 ) )
    .WillOnce( Return( 50 ) )
    .WillOnce( Return( 0 ) )
    .WillOnce( Return( 50 ) )
    .WillOnce( Return( 34 ) )
    .WillOnce( Return( 50 ) )
    .WillOnce( Return( 0 ) ); 

    int serialCount = Serial1.available();
    do {
        pLogger->print( "Serial available: " );
        pLogger->print( serialCount );
        pLogger->println( "" );
        MidiMessage serialMsg( &Serial1 );
        pLogger->print( " INPUT: " );
        serialMsg.Report( pLogger );
        serialCount = Serial1.available();
    } while ( serialCount > 0 );
    
    pLogger->println( "Serial empty" );

}

// Midi Message Tests

TEST( midiMessageT, testManualConstructor ) {

    MidiMessage mm( 144, 50, 34 );

    mm.Report( pLogger );

    EXPECT_EQ( mm.GetChannel(), 1 );
    EXPECT_EQ( mm.GetMessageType(), NOTE_ON );
    EXPECT_EQ( mm.GetDataByte2(), 50 );
    EXPECT_EQ( mm.GetDataByte3(), 34 );

}

TEST( midiMessageT, testMockSerialReturnsValue ) {

    EXPECT_CALL( Serial1, read() )
    .Times( 1 )
    .WillOnce( Return( 144 ) );

    uint8_t aByte = Serial1.read();

    std::cout<<aByte<<std::endl;

    EXPECT_EQ( aByte, 144 );

}

TEST( midiMessageT, testMockSerialReturnsAvailable ) {

    EXPECT_CALL( Serial1, available() )
    .WillOnce( Return( 22 ) );

    int avail = Serial1.available();

    std::cout<<avail<<std::endl;

    EXPECT_EQ( avail, 22 );

}

TEST( midiMessageT, testConstructorFromSerial ) {

    EXPECT_CALL( Serial1, available() )
    .WillOnce( Return( 3 ) )
    .WillOnce( Return( 2 ) )
    .WillOnce( Return( 1 ) )
    .WillRepeatedly(Return( 0 ) );
    
    EXPECT_CALL( Serial1, read() )
    .WillOnce( Return( 144 ) )
    .WillOnce( Return( 50 ) )
    .WillOnce( Return( 34 ) )
    .WillRepeatedly(Return( 0 ) );

    MidiMessage mm( &Serial1 );

    mm.Report( pLogger );

    EXPECT_EQ( mm.GetChannel(), 1 );
    EXPECT_EQ( mm.GetMessageType(), NOTE_ON );
    EXPECT_EQ( mm.GetDataByte2(), 50 );
    EXPECT_EQ( mm.GetDataByte3(), 34 );

}

TEST_F( lcdBufferT, Instantiate ) {
    EXPECT_EQ( mockLcd.initialised, true );
    EXPECT_CALL( mockLcd, begin( 16, 2 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, setCursor( 0, 0 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, clear() ).WillOnce( Return() );

    // create buffer with fake lcd
    LcdBuffer lcdb( &mockLcd, pLogger, 16, 2 );
    char buffer[ 33 ];
    buffer[ 0 ] = '\0';
    lcdb.GetBufferContents( buffer );
    EXPECT_STREQ( buffer, "                \n                \n");
}

TEST_F( lcdBufferT, BufferUpdate ) {
    EXPECT_EQ( mockLcd.initialised, true );
    EXPECT_CALL( mockLcd, begin( 16, 2 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, setCursor( 0, 0 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, clear() ).WillOnce( Return() );

    // create buffer with fake lcd
    LcdBuffer lcdb( &mockLcd, pLogger, 16, 2 );

    lcdb.UpdateBuffer( 0, "PROGRAM CHANGER" );
    char buffer[ 33 ];
    buffer[ 0 ] = '\0';
    lcdb.GetBufferContents( buffer );
    EXPECT_STREQ( buffer, "PROGRAM CHANGER \n                \n");
}

TEST_F( lcdBufferT, DisplayUpdate1 ) {
    EXPECT_EQ( mockLcd.initialised, true );
    EXPECT_CALL( mockLcd, begin( 16, 2 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, setCursor( 0, 0 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, clear() ).WillOnce( Return() );

    // create buffer with fake lcd
    LcdBuffer lcdb( &mockLcd, pLogger, 16, 2 );
   
    lcdb.UpdateBuffer( 0, "PROGRAM CHANGER" );
    char buffer[ 33 ];
    buffer[ 0 ] = '\0';
    lcdb.GetBufferContents( buffer );
    EXPECT_STREQ( buffer, "PROGRAM CHANGER \n                \n");
    char display[ 33 ];
    display[ 0 ] = '\0';
    lcdb.GetDisplayContents( display );
    EXPECT_STREQ( display, "                \n                \n");
    EXPECT_CALL( mockLcd, setCursor( _, _ ) ).Times( 14 );
    EXPECT_CALL( mockLcd, print( _ ) ).Times( 14 );
    lcdb.UpdateLcdFromBuffer();
    display[ 0 ] = '\0';
    lcdb.GetDisplayContents( display );
    EXPECT_STREQ( display, "PROGRAM CHANGER \n                \n");
}

TEST_F( lcdBufferT, DisplayUpdate2 ) {
    EXPECT_EQ( mockLcd.initialised, true );
    EXPECT_CALL( mockLcd, begin( 16, 2 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, setCursor( 0, 0 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, clear() ).WillOnce( Return() );

    // create buffer with fake lcd
    LcdBuffer lcdb( &mockLcd, pLogger, 16, 2 );
   
    lcdb.UpdateBuffer( 0, "PROGRAM CHANGER" );
    char buffer[ 33 ];
    buffer[ 0 ] = '\0';
    lcdb.GetBufferContents( buffer );
    EXPECT_STREQ( buffer, "PROGRAM CHANGER \n                \n");
    char display[ 33 ];
    display[ 0 ] = '\0';
    lcdb.GetDisplayContents( display );
    EXPECT_STREQ( display, "                \n                \n");
    EXPECT_CALL( mockLcd, setCursor( _, _ ) ).Times( 14 );
    EXPECT_CALL( mockLcd, print( _ ) ).Times( 14 );
    lcdb.UpdateLcdFromBuffer();
    display[ 0 ] = '\0';
    lcdb.GetDisplayContents( display );
    EXPECT_STREQ( display, "PROGRAM CHANGER \n                \n");

    lcdb.UpdateBuffer( 0, "X", 0, 1 );
    buffer[ 0 ] = '\0';
    lcdb.GetBufferContents( buffer );
    EXPECT_STREQ( buffer, "XROGRAM CHANGER \n                \n");
    display[ 0 ] = '\0';
    lcdb.GetDisplayContents( display );
    EXPECT_STREQ( display, "PROGRAM CHANGER \n                \n");
    EXPECT_CALL( mockLcd, setCursor( _, _ ) ).Times( 1 );
    EXPECT_CALL( mockLcd, print( _ ) ).Times( 1 );
    lcdb.UpdateLcdFromBuffer();
    display[ 0 ] = '\0';
    lcdb.GetDisplayContents( display );
    EXPECT_STREQ( display, "XROGRAM CHANGER \n                \n");

}

TEST_F( lcdBufferT, ReportBufferContents ) {
    EXPECT_EQ( mockLcd.initialised, true );
    EXPECT_CALL( mockLcd, begin( 16, 2 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, setCursor( 0, 0 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, clear() ).WillOnce( Return() );


    // create buffer with fake lcd
    LcdBuffer lcdb( &mockLcd, pLogger, 16, 2 );
   
    lcdb.UpdateBuffer( 0, "PROGRAM CHANGER" );
    char buffer[ 33 ];
    buffer[ 0 ] = '\0';
    lcdb.GetBufferContents( buffer );
    EXPECT_STREQ( buffer, "PROGRAM CHANGER \n                \n");

    lcdb.ReportBufferContents();

}

TEST_F( lcdBufferT, ReportDisplayContents ) {
    EXPECT_EQ( mockLcd.initialised, true );
    EXPECT_CALL( mockLcd, begin( 16, 2 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, setCursor( 0, 0 ) ).WillOnce( Return() );
    EXPECT_CALL( mockLcd, clear() ).WillOnce( Return() );

    // create buffer with fake lcd
    LcdBuffer lcdb( &mockLcd, pLogger, 16, 2 );
   
    lcdb.UpdateBuffer( 0, "PROGRAM CHANGER" );
    char buffer[ 33 ];
    buffer[ 0 ] = '\0';
    lcdb.GetBufferContents( buffer );
    EXPECT_STREQ( buffer, "PROGRAM CHANGER \n                \n");

    char display[ 33 ];
    display[ 0 ] = '\0';
    lcdb.GetDisplayContents( display );
    EXPECT_STREQ( display, "                \n                \n");
    EXPECT_CALL( mockLcd, setCursor( _, _ ) ).Times( 14 );
    EXPECT_CALL( mockLcd, print( _ ) ).Times( 14 );
    lcdb.UpdateLcdFromBuffer();
    display[ 0 ] = '\0';
    lcdb.GetDisplayContents( display );
    EXPECT_STREQ( display, "PROGRAM CHANGER \n                \n");
    
    lcdb.ReportDisplayContents();

}

// Midi Message Translation Map Tests

TEST_F( midiMessageTranslationMapT, canInstantiate ) {

    EXPECT_EQ( mmtm->GetSize(), 0 );

}

TEST_F( midiMessageTranslationMapT, canInsert ) {

    EXPECT_EQ( mmtm->GetSize(), 0 );

    MidiMessageTranslation mmt;
    mmt.SetChannelPair( 1, 2 );
    mmt.SetMessageTypePair( NOTE_ON, NOTE_ON );
    EXPECT_THAT( mmt.GetChannelPair(), Pair( 1, 2 ) );

    mmtm->InsertTranslation( mmt );
    EXPECT_EQ( mmtm->GetSize(), 1 );

}

TEST_F( midiMessageTranslationMapT, checkExistsAfterInsert ) {

    MidiMessageTranslation mmt;
    mmt.SetChannelPair( 1, 2 );
    mmt.SetMessageTypePair( NOTE_ON, NOTE_ON );

    // 144 10010000 Note On, Channel 1
    EXPECT_EQ( mmt.GetStatusByte(), 144);

    mmtm->InsertTranslation( mmt );
    EXPECT_EQ( mmtm->TranslationExists( 144 ), true );

}

TEST_F( midiMessageTranslationMapT, checkTranslationForChannel1To2 ) {

    MidiMessageTranslation mmt;
    mmt.SetChannelPair( 1, 2 );
    mmt.SetMessageTypePair( NOTE_ON, NOTE_ON );

    mmtm->InsertTranslation( mmt );

    MidiMessage mm{ 144, 40, 64 };
    mmtm->Translate( &mm );

    // 145 10010010 Note On, Channel 2
    EXPECT_EQ( mm.GetStatusByte(), 145 );
    EXPECT_EQ( mm.GetChannel(), 2 );
    EXPECT_EQ( mm.GetDataByte2(), 40 );   
    EXPECT_EQ( mm.GetDataByte3(), 64 );
}

TEST_F( midiMessageTranslationMapT, checkTranslationForNoteOnToControlChange ) {
    
    // 146 10010010 Note On, Channel 3
    MidiMessageTranslation mmt;
    mmt.SetChannelPair( 3, 3 );
    mmt.SetMessageTypePair( NOTE_ON, CONTROL_CHANGE );

    mmtm->InsertTranslation( mmt );

    MidiMessage mm{ 146, 40, 64 };
    mmtm->Translate( &mm );

    // 178 10110011 Control Change, Channel 3
    EXPECT_EQ( mm.GetStatusByte(), 178 );
    EXPECT_EQ( mm.GetChannel(), 3 );
    EXPECT_EQ( mm.GetDataByte2(), 40 );   
    EXPECT_EQ( mm.GetDataByte3(), 64 );
}

TEST_F( midiMessageTranslationMapT, checkTranslationForDataByte2 ) {
    
    // 144 10010010 Note On, Channel 1
    MidiMessageTranslation mmt;
    mmt.SetChannelPair( 1, 1 );
    mmt.SetMessageTypePair( NOTE_ON, NOTE_ON );
    mmt.AddDataByte2MapPair( 40, 50 );

    std::map < uint8_t, uint8_t > m;
    m.insert( std::make_pair( 40, 50 ) );
    EXPECT_EQ( mmt.GetDataByte2(), m );

    mmtm->InsertTranslation( mmt );

    MidiMessage mm{ 144, 40, 64 };
    mmtm->Translate( &mm );

    EXPECT_EQ( mm.GetStatusByte(), 144 );
    EXPECT_EQ( mm.GetChannel(), 1 );
    EXPECT_EQ( mm.GetDataByte2(), 50 );   
    EXPECT_EQ( mm.GetDataByte3(), 64 );
}

TEST_F( midiMessageTranslationMapT, checkTranslationForDataByte3 ) {
    
    // 144 10010010 Note On, Channel 1
    MidiMessageTranslation mmt;
    mmt.SetChannelPair( 1, 1 );
    mmt.SetMessageTypePair( NOTE_ON, NOTE_ON );
    mmt.AddDataByte3MapPair( 64, 127 );

    std::map < uint8_t, uint8_t > m;
    m.insert( std::make_pair( 64, 127 ) );
    EXPECT_EQ( mmt.GetDataByte3(), m );

    mmtm->InsertTranslation( mmt );

    MidiMessage mm{ 144, 40, 64 };
    mmtm->Translate( &mm );

    EXPECT_EQ( mm.GetStatusByte(), 144 );
    EXPECT_EQ( mm.GetChannel(), 1 );
    EXPECT_EQ( mm.GetDataByte2(), 40 );   
    EXPECT_EQ( mm.GetDataByte3(), 127 );
}

// Midi Message Filter Tests

TEST( midiMessageFilterT, canGetandSet ) {
    MidiMessageFilter msgFilter( pLogger );
    msgFilter.SetChannelFilter( 1, false );
    EXPECT_EQ( false, msgFilter.GetChannelFilter( 1 ) );

    msgFilter.SetChannelFilter( 2, true );
    EXPECT_EQ( false, msgFilter.GetChannelFilter( 1 ) );
    
    EXPECT_EQ( false, msgFilter.GetChannelFilter( 3 ) );

    msgFilter.SetMessageTypeFilter( NOTE_ON, true );
    EXPECT_EQ( true, msgFilter.GetMessageTypeFilter( NOTE_ON ) );

    EXPECT_EQ( false, msgFilter.GetMessageTypeFilter( NOTE_OFF ) );

}

TEST( midiMessageFilterT, getFilterAsXmlString )  {

    // Create object and set test values
    MidiMessageFilter msgFilter( pLogger );
    msgFilter.SetChannelFilter( 1, false );
    msgFilter.SetChannelFilter( 2, true );
    msgFilter.SetMessageTypeFilter( NOTE_ON, true );

    // Get object as standard string and parse back into XML object
    std::string xmlString = msgFilter.GetFilterAsXmlString();
    const char * cstr = xmlString.c_str();
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError eResult = xmlDoc.Parse( cstr );
    EXPECT_EQ( tinyxml2::XML_SUCCESS, eResult );

    // ensure that document has root node
    tinyxml2::XMLNode * pRoot = xmlDoc.FirstChild();
    ASSERT_NE( pRoot, nullptr );

    // ensure root node has correct name
    std::string pRootValue = pRoot->Value();
    EXPECT_EQ( pRootValue, "MidiMessageFilter" );

    // test for the second node
    tinyxml2::XMLNode * pChannelFilter = pRoot->FirstChild();
    ASSERT_NE( pChannelFilter, nullptr );
    
    // test the name of the second node
    std::string pChannelFilterValue = pChannelFilter->Value();
    EXPECT_EQ( pChannelFilterValue, "ChannelFilter" );

    // ensure second node has element
    tinyxml2::XMLElement * pChannelFilterElement = pChannelFilter->FirstChildElement();
    ASSERT_NE( pChannelFilterElement, nullptr );

    // test the name of the element
    std::string pChannelFilterElementValue = pChannelFilterElement->Value();
    EXPECT_EQ( pChannelFilterElementValue, "Filter" );
    
    // test the first attribute of the element against the test values
    int outInt;
    eResult = pChannelFilterElement->QueryIntAttribute( "channel", &outInt );
    EXPECT_EQ( eResult, tinyxml2::XML_SUCCESS );
    EXPECT_EQ( outInt, 1 );
    
    // test the second attribute of the element against the test values
    bool outBool;
    eResult = pChannelFilterElement->QueryBoolAttribute( "status", &outBool );
    EXPECT_EQ( eResult, tinyxml2::XML_SUCCESS );
    EXPECT_EQ( outBool, false );

    // ensure second node has another element 
    pChannelFilterElement = pChannelFilterElement->NextSiblingElement();
    ASSERT_NE( pChannelFilterElement, nullptr );

    // test the name of the element
    pChannelFilterElementValue = pChannelFilterElement->Value();
    EXPECT_EQ( pChannelFilterElementValue, "Filter" );
    
    // test the first attribute of the element against the test values
    eResult = pChannelFilterElement->QueryIntAttribute( "channel", &outInt );
    EXPECT_EQ( eResult, tinyxml2::XML_SUCCESS );
    EXPECT_EQ( outInt, 2 );

    // test the second attribute of the element against the test values
    eResult = pChannelFilterElement->QueryBoolAttribute( "status", &outBool );
    EXPECT_EQ( eResult, tinyxml2::XML_SUCCESS );
    EXPECT_EQ( outBool, true );

    // test for the third node 
    tinyxml2::XMLNode * pMessageTypeFilter = pChannelFilter->NextSibling();
    ASSERT_NE( pChannelFilterElement, nullptr );

    // test the name of the third node
    std::string pMessageTypeFilterValue = pMessageTypeFilter->Value();
    EXPECT_EQ( pMessageTypeFilterValue, "MessageTypeFilter" );

}

TEST( midiMessageFilterT, setFilterFromXmlString ) {

    // Create object and check it exists
    MidiMessageFilter msgFilter( pLogger );
    ASSERT_EQ( msgFilter.GetChannelFilter( 1 ), false );
    
    // Create XMLDocument from file
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError eResult = xmlDoc.LoadFile("M_FILT_T.XML"); // from same directory as test binary
    EXPECT_EQ( eResult, tinyxml2::XML_SUCCESS );

    std::string xmlString = msgFilter.GetXmlObjectAsStdString( &xmlDoc );
    // std::cout<<xmlString<<std::endl;

    EXPECT_EQ( msgFilter.SetFilterFromXmlString( xmlString ), tinyxml2::XML_SUCCESS );
    
    EXPECT_EQ( msgFilter.GetChannelFilter( 1 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 2 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 3 ), true );
    EXPECT_EQ( msgFilter.GetChannelFilter( 4 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 5 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 6 ), true );
    EXPECT_EQ( msgFilter.GetChannelFilter( 7 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 8 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 9 ), true );
    EXPECT_EQ( msgFilter.GetChannelFilter( 10 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 11 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 12 ), true );
    EXPECT_EQ( msgFilter.GetChannelFilter( 13 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 14 ), false );
    EXPECT_EQ( msgFilter.GetChannelFilter( 15 ), true );
    EXPECT_EQ( msgFilter.GetChannelFilter( 16 ), false );

    EXPECT_EQ( msgFilter.GetMessageTypeFilter( NOTE_OFF ), true );
    EXPECT_EQ( msgFilter.GetMessageTypeFilter( NOTE_ON ), false );
    EXPECT_EQ( msgFilter.GetMessageTypeFilter( KEY_PRESSURE ), false );
    EXPECT_EQ( msgFilter.GetMessageTypeFilter( CONTROL_CHANGE ), false );
    EXPECT_EQ( msgFilter.GetMessageTypeFilter( PROGRAM_CHANGE ), true );
    EXPECT_EQ( msgFilter.GetMessageTypeFilter( CHANNEL_PRESSURE ), false );
    EXPECT_EQ( msgFilter.GetMessageTypeFilter( PITCH_BEND ), false );
    EXPECT_EQ( msgFilter.GetMessageTypeFilter( SYSTEM_MESSAGE ), false );
    EXPECT_EQ( msgFilter.GetMessageTypeFilter( 16 ), true );

}

TEST( midiMessageFilterT, checkReportFunction ) {

    // Create object and check it exists
    MidiMessageFilter msgFilter( pLogger );
    ASSERT_EQ( msgFilter.GetChannelFilter( 1 ), false );
    
    // Create XMLDocument from file
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError eResult = xmlDoc.LoadFile("M_FILT_T.XML"); // from same directory as test binary
    EXPECT_EQ( eResult, tinyxml2::XML_SUCCESS );

    std::string xmlString = msgFilter.GetXmlObjectAsStdString( &xmlDoc );
    // std::cout<<xmlString<<std::endl;

    EXPECT_EQ( msgFilter.SetFilterFromXmlString( xmlString ), tinyxml2::XML_SUCCESS );

    msgFilter.Report();

}