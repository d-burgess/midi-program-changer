#include "GuiElement.h"

GuiElement::GuiElement(
    Logger * logger, 
    uint8_t mn,
    uint8_t mx,
    uint8_t r,
    uint8_t c,
    uint8_t d
    )
        : pLog( logger ),
        min( mn ),
        max( mx ),
        row( r ),
        column( c ),
        digits( d )
{
    value = mn;
} // constructor

GuiElement::~GuiElement() {

}

void GuiElement::increment() {
    ( value < max ) ? ++value : value = min;
}

void GuiElement::decrement() {
    ( value > min ) ? --value : value = max;
}

uint8_t GuiElement::getValue() {
    return value;
}

uint8_t GuiElement::getRow() {
    return row;
}

uint8_t GuiElement::getColumn() {
    return column;
}

uint8_t GuiElement::getDigits() {
    return digits;
}