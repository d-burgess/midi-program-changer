/*

Button Event Buffer
FIFO

*/

#ifndef __MPCButtonEventBuffer__
#define __MPCButtonEventBuffer__

#ifndef TEENSYDUINO
    // #include "fake/Serial.h"
    // #include "fake/HardwareSerial.h"
    #include "fake/LiquidCrystalFast.h"
    // #include "fake/String.h"
    // extern Serial_ Serial;
#else
    #include "Arduino.h"
#endif

#include <stdint.h>

#define BUFFER_SIZE 64

class ButtonEventBuffer {
  private:
    uint8_t first;
    uint8_t last;
    uint8_t count;
    uint8_t buffer[ BUFFER_SIZE ];
  public:
    ButtonEventBuffer();
    ~ButtonEventBuffer();
    void Push( uint8_t event );
    uint8_t Pop();
    const uint8_t Count();
};

#endif
