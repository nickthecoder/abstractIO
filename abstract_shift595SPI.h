/*
Shift register

This makes use of the SPI library, and because the Arduino IDE is trying cleverer than
it is, that causes a big problem! To get around this problem, I use a horrible bodge...
To include this code into your sketch you need to include the following :

#include <SPI.h>
#include <abstract_shift595SPI.h>
#include <abstract_shift595SPI.cpp.h>

If you have split your code into different files, only include <abstract_shift595SPI.cpp.h> once.

What this does, is compile this code into YOUR sketch, rather than into the abstractIO library.
This makes it nice and simple for most people, and just a bit weird for people who want to use
Shift595SPI.

This is a horrible bodge, and I know it, so if you know of a better way to do it, let me know!
(I don't want to make everybody #include <SPI.h> when they aren't using it).
*/
#ifndef abstract_Shift595SPI_h
#define abstract_Shift595SPI_h

#include <Arduino.h>
#include "abstractIO.h"

class Shift595SPI {

  public :
    byte latchPin;
    
    byte* buffer;
    int byteCount;
    boolean bitOrder; // Either MSBFIRST or LSBFIRST
   
  public :
    Shift595SPI( int latchPin, int byteCount = 1 );

    void update();
};

#endif

