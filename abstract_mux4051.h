#ifndef abstract_mux4051_h
#define abstract_mux4051_h

#include <Arduino.h>
#include "abstractIO.h"

class Mux4051 : public Selector
{
  private :
    byte a0Pin, a1Pin, a2Pin; // Address pins
    byte chipEnablePin;

  public :
    Mux4051( byte a0Pin, byte a1Pin, byte a2Pin, byte chipEnablePin = -1 );
    
    void select( int address );
    void enable( boolean value );

};

/*
  Controls a pair of 4051s with a single chip-select from the arduino.
  Bits 0,1,2 of the address are used as normal, and bit 3 is used to select which of the two chips
  are enabled.
  Wire the chipEnable pin directly to the first 4051's chip-enable pin, and the second 
  via an inverter.
*/
class Mux4051Pair : public Selector
{
  private :
    Mux4051* wrapped;

  public :
    Mux4051Pair( int a0Pin, int a1Pin, int a2Pin, int a3Pin );
    virtual void select( int address );
};

#endif

