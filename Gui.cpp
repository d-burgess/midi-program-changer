#include "Gui.h"

Gui::Gui(
    LcdBuffer * lcdBuffer, 
    Logger * logger
    )
        : pLcdBuffer( lcdBuffer ),
        pLogger( logger ) 
{

} // constructor

Gui::~Gui() {

}

void Gui::addElement( GuiElement * e ) {
    ++numElements;
    element[ numElements ] = e;
}

uint8_t Gui::elementCount() {
    return numElements;
}

void Gui::update() {
    for ( size_t i = 1; i <= elementCount(); ++i ) {
        uint8_t value { element[ i ]->getValue() };
        uint8_t row { element[ i ]->getRow() };
        uint8_t column { element[ i ]->getColumn() };
        uint8_t length { element[ i ]->getDigits() };
        char msg[ length + 1 ] { '\0' };
        padByteWithLeadingZeros( msg, value, length );
        pLcdBuffer->UpdateBuffer( row, msg, column, length );
    }

    switch ( currentMode ) {
    case play:
        pLcdBuffer->writeCustomChar( LCD_CHAR_NOTE, 15, 0 );
        break;
    case freeze:
        pLcdBuffer->writeCustomChar( LCD_CHAR_FREEZE, 15, 0 );
        break;
    case thru:
        pLcdBuffer->writeCustomChar( LCD_CHAR_THRU, 15, 0 );
        break;
    case save:
        pLcdBuffer->writeCustomChar( LCD_CHAR_SAVED, 15, 0 );
        break; 
    case edit:
        pLcdBuffer->writeCustomChar( LCD_CHAR_EDIT, 15, 0 );
        break;
    default:
        break;
    }

}

void Gui::padByteWithLeadingZeros( char * ref, uint8_t aByte, uint8_t totalLength ) {
    char formatter[ totalLength + 5 ] { '\0' };
    strcat( formatter, "%0" );
    char lengthAsString[ totalLength + 1 ] { '\0' };
    snprintf( lengthAsString, 4, "%d", totalLength );
    strcat( formatter, lengthAsString );
    strcat( formatter, "d" );
    snprintf( ref, totalLength + 1, formatter, aByte );
}

void Gui::incrementElement( uint8_t e ) {
    element[ e ]->increment();
}

void Gui::incrementSelectedElement() {
    element[ selectedElement ]->increment();
}

void Gui::decrementSelectedElement() {
    element[ selectedElement ]->decrement();
}

void Gui::selectNextElement() {
    ( selectedElement < elementCount() ) ? ++selectedElement : selectedElement = 1;
}

void Gui::setCursorToCurrentElement() {
    pLcdBuffer->setCursor( element[ selectedElement ]->getColumn() + element[ selectedElement ]->getDigits() - 1, element[ selectedElement ]->getRow() );
    pLcdBuffer->cursor();
    // pLcdBuffer->blink();
}

void Gui::cycleMode() {
    pLogger->print( " cycleMode " );
    if ( currentMode == play ) {
        pLogger->print( "play" );
        currentMode = freeze;
    } else if ( currentMode == freeze ) {
        pLogger->print( "freeze" );
        currentMode = thru;
    } else if ( currentMode == thru ) {
        pLogger->print( "thru" );
        currentMode = save;
    } else if ( currentMode == save ) {
        pLogger->print( "save" );
        currentMode = edit;
    } else if ( currentMode == edit ) {
        pLogger->print( "edit" );
        currentMode = play;
    }
    pLogger->print( static_cast<uint8_t>( currentMode ) );
    pLogger->println( " " );
    
}

mpcMode Gui::getCurrentMode() 
{
    return currentMode;
}