/*

Hardware Serial

*/

#if defined( TEENSYDUINO ) 
  #if defined( __MK20DX256__ )       
    #define BOARD "Teensy 3.2"
    #define HWSERIAL Serial1
  #endif
#elif defined( ARDUINO_AVR_UNO )       
  #define BOARD "Arduino Uno"
  #define HWSERIAL Serial
#else
  #define BOARD "Unknown"
  #define HWSERIAL Serial
#endif