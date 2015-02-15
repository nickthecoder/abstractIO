#ifndef abstract_shift164_h
#define abstract_shift164_h

#include "abstractIO.h"

/*
Use a shift register to act as a chip selector.
Uses just 2 pins from the Arduino.

We can arrange for the shift registers's outputs to have all zeros except one -
for the one chip we wish to enable.

Note: Multiple 164's can be chained together, so the chipCount can be over 8.

It will take Order(n) time to select one of n chips. Therefore, if you are controlling
less than 8 chips, change the default chipCount parameter in the constructor.
*/
class Shift164Selector : public Selector
{
  private :
    byte chipCount; // The number of output channels of our chip selector.
    byte clockPin, dataPin;

  public :
    Shift164Selector( byte clockPin, byte dataPin, byte chipCount = 8 );

    void select( int address );
};

#endif

