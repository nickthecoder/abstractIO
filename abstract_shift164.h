#ifndef abstract_shift164_h
#define abstract_shift164_h

#include "abstractIO.h"

/*
Use a shift register to act as a chip selector.
Uses just 2 pins from the Arduino.

We can arrange for the shift registers's outputs to have all zeros except one -
for the one chip we wish to enable.

Note: Multiple 164's can be chained together, so the chipCount can be over 8.
*/
class Shift164Selector : public Selector
{
  protected :
    byte addressLines; // The number of output channels of our chip selector.
    byte clockPin, dataPin;
    byte previousAddress;

  protected :
    Shift164Selector( byte clockPin, byte dataPin, byte addressLines, boolean initialise );

  public :
    Shift164Selector( byte clockPin, byte dataPin, byte addressLines = 8 )
      : Shift164Selector( clockPin, dataPin, addressLines, true ) {};

    virtual void select( int address );

  protected :
    void shift( int n );
    void initialise();
};

#endif

