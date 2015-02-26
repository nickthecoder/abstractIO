/*
Shift register

NOTE. In your sketch add :
#include <SPI.h>
This is because this library has the *option* of using SPI to transfer the data, and
so even if you choose not to use SPI, for the code to compile, it needs the SPI library.
Alas, the Arduino IDE isn't clever enough to know that SPI needs to be included in the build
unless YOU put it there, despite the fact that this library already includes it. Grrr!
*/
#ifndef abstract_Shift595_h
#define abstract_Shift595_h

#include <Arduino.h>
#include "abstractIO.h"
#include "abstract_shift164.h"

class Shift595
{

  public :
    byte clockPin;
    byte dataPin;
    byte latchPin;
    
    byte* buffer;
    int byteCount;
    boolean bitOrder; // Either MSBFIRST or LSBFIRST
   
  public :
    Shift595( int latchPin, int clockPin, int dataPin, int byteCount = 1 );

    void update();
};

/*
Use a shift register to act as a chip selector.

We can arrange for the shift registers's outputs to have all zeros except one -
for the one chip we wish to enable.

Note: Multiple 595's can be chained together, so the chipCount can be over 8.

Inherrits from Shift164Selector, which does basically the same thing, but the 595
has a latch.
*/
class Shift595Selector : public Shift164Selector
{

  public :
    byte latchPin;
       
  public :
    Shift595Selector( int latchPin, int clockPin, int dataPin, int addressLines = 8, byte activeHighLow = LOW );

    virtual void select( int address );
};

#endif

