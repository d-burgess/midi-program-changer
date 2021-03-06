/*

Midi Message FIFO Buffer

*/

#ifndef __MidiMessageFIFOBuffer__
#define __MidiMessageFIFOBuffer__

#include "MidiMessage.h"

#include "Arduino.h"

#define BUFFER_SIZE 64

class MidiMessageFIFOBuffer {
  private:
    int first;
    int last;
    int count;
    MidiMessage buffer[ BUFFER_SIZE ];
  public:
    MidiMessageFIFOBuffer();
    ~MidiMessageFIFOBuffer();
    void Push( MidiMessage msg );
    MidiMessage Pop();
    const int Count();
};

#endif
