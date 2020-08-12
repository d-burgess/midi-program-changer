/*

Midi Message FIFO Buffer

*/

#include "MidiMessageFIFOBuffer.h"

MidiMessageFIFOBuffer::MidiMessageFIFOBuffer() {
    // constructor
    first = 0;
    last = 0;
    count = 0;
}

MidiMessageFIFOBuffer::~MidiMessageFIFOBuffer() {
    // destructor
}

void MidiMessageFIFOBuffer::Push( MidiMessage msg ) {
    if ( count >= BUFFER_SIZE ) {
        return; // buffer is full
    } else {
        count++; // one more message in the buffer

        if( count > 1 ) { // only move the last if there is more than one element
            last++; // move last location up one
            last %= BUFFER_SIZE; // if last exceeds buffer size, set it to the remainder
        }

        buffer[ last ] = msg; // store MidiMessage object into array
    }
}

MidiMessage MidiMessageFIFOBuffer::Pop() {
    if ( count == 0 ) {
        return MidiMessage(); // nothing in the buffer, return empty midi message
    } else {
        count--; // one less message in the buffer

        MidiMessage msg = buffer[ first ];

        if(count >= 1) {
            first++; // move first location up one
            first %= BUFFER_SIZE; // if first exceeds buffer size, set it to the remainder
        }

        return msg;
    }
}

const int MidiMessageFIFOBuffer::Count() {
    return count;
}
