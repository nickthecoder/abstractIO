/*
Shift register
*/
#ifndef abstract_M74HC59_h
#define abstract_M74HC59_h

#include <Arduino.h>

class M74HC59 {

  public :
    byte shiftClockPin;
    byte storageClockPin;
    byte dataPin;
    
    boolean* buffer;
    int outputCount;

  public :
    M74HC59( int outputCount = 8 );

    M74HC59* shiftClock( byte pin )   { shiftClockPin   = pin; pinMode( pin, OUTPUT); digitalWrite( pin, HIGH ); return this; };
    M74HC59* storageClock( byte pin ) { storageClockPin = pin; pinMode( pin, OUTPUT); digitalWrite( pin, HIGH ); return this; };
    M74HC59* data( byte pin )         { dataPin         = pin; pinMode( pin, OUTPUT); return this; };

    void update();
};



#endif

