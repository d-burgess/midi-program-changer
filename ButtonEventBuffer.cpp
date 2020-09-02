/*

Button Event Buffer
FIFO

*/

#include "ButtonEventBuffer.h"

ButtonEventBuffer::ButtonEventBuffer() 
    : first { 0 },
    last { 0 },
    count { 0 }
{
    // constructor
}

ButtonEventBuffer::~ButtonEventBuffer() {
    // destructor
}

void ButtonEventBuffer::Push( uint8_t event ) {
    if ( count >= BUFFER_SIZE ) {
        return; // buffer is full
    } else {
        ++count; // one more event in the buffer

        if( count > 1 ) { // only move the last if there is more than one element
            ++last; // move last location up one
            last %= BUFFER_SIZE; // if last exceeds buffer size, set it to the remainder
        }

        buffer[ last ] = event; // store event into array
    }
}

uint8_t ButtonEventBuffer::Pop() {
    if ( count == 0 ) {
        return 0; // nothing in the buffer, return empty event: EVENT_NO_EVENT
    } else {
        --count; // one less event in the buffer

        uint8_t event = buffer[ first ];

        if ( count >= 1)  {
            ++first; // move first location up one
            first %= BUFFER_SIZE; // if first exceeds buffer size, set it to the remainder
        }

        return event;
    }
}

const uint8_t ButtonEventBuffer::Count() {
    return count;
}
