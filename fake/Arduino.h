#include <ctime>

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

typedef uint8_t byte;

time_t micros( void );

void delay( time_t t );

unsigned long millis ( void );

void pinMode( uint8_t pin, uint8_t mode );

int digitalRead( uint8_t pin );