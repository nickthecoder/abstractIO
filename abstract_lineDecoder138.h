#ifndef abstract_lineDecoder138_h
#define abstract_lineDecoder138_h

#include "abstractIO.h"

/*
  A 138 line decoder, takes a 3 bit address, and converts it into 8 output lines.
  These are typically used to control chip-select pins, and therefore selecting
  one of (upto) 8 chips.
  
  Note. There is currently no attempt to make use of the three enable pins, which would
  allow multiple decoders to be used in parallel.
*/
class LineDecoder138 : public Selector
{
  private :
    byte a0, a1, a2;

  public :
    LineDecoder138( byte a0, byte a1 = ABSTRACT_NOT_USED, byte a2 = ABSTRACT_NOT_USED );

    void select( int address );
};

#endif

