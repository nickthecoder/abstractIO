/*
Shift register
*/
#ifndef abstract_M74HC595_h
#define abstract_M74HC595_h

#include <Arduino.h>

class M74HC595 {

  public :
    byte clockPin;
    byte latchPin;
    byte dataPin;
    
    boolean* buffer;
    int outputCount;

  public :
    M74HC595( int outputCount = 8 );

    M74HC595* shiftClock( byte pin )   { clockPin = pin; pinMode( pin, OUTPUT); digitalWrite( pin, HIGH ); return this; };
    M74HC595* storageClock( byte pin ) { latchPin = pin; pinMode( pin, OUTPUT); digitalWrite( pin, HIGH ); return this; };
    M74HC595* data( byte pin )         { dataPin  = pin; pinMode( pin, OUTPUT); return this; };

    void update();
};



#endif

