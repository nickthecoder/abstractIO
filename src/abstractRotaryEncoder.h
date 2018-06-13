/*
 * A rotary encoder is a tricky beast, because if you poll the outputs (i.e. check the outputs once per loop() ),
 * then it is very easy to miss a beat if the loop is longer than the time between beats.
 * Therefore only use SimpleRotaryEncoder if your loop() is very quick, or if you don't mind when beats are missed.
 * 
 * A common solution is to use interrupts to monitor the RE, however, the Arduino only has 2 interrupts,
 * and therefore can only handle a single encoder. Alternatively a timer can also be used to monitor multiple
 * encoders. I may well use this method here later...
 * 
 * However, my long term solution involves a small PIC microcontroller to monitor the rotartary encoder
 * which the Arduino can talk to via an I2C interface. Small PICs are less than £1 from China (47p each),
 * so the extra hardware isn't expensive.
 * You can then have as many encoders as you like by changing the I2C address on each PIC.
 * The cheap 12F686 has 6 data pins. 2 for I2C and three for the encoder (including a push button), leaving a spare pin,
 * which could be used for a status LED for each encoder.
 */

#ifndef abstractRotaryEncoder_h
#define abstractRotaryEncoder_h

#include <Arduino.h>
#include "abstractIO.h"

class RotaryEncoder;
class SimpleRotaryEncoder;
class REAnalogInput;

class RotaryEncoder
{
  public :
    // Returns the value of the encoder.
    virtual int get();
    virtual void set( int value );
    
    REAnalogInput* createAnalogInput( int steps );
};

/*
 * Polls a rotary encoder every time get() is called.
 * This is a flawed approach, because it is too easy to miss steps, when the time between
 * the calls is larger than the period of a step.
 */
class SimpleRotaryEncoder : public RotaryEncoder
{
  public :
    // beatsPerDetent ensures that each "click" (detent) changes the value from get() by 1.
    SimpleRotaryEncoder( byte pinA, byte pinB, byte beatsPerDetent = 2 );
    virtual int get();
    virtual void set( int value );

  protected :
    byte pinA;
    byte pinB;
    int val;
    byte beatsPerDetent;
    boolean aState;
};

/**
 * Makes a rotary encode appear like an analog input (returning a float in the range 0..1).
 */
class REAnalogInput : public AnalogInput
{
  public :
    REAnalogInput( RotaryEncoder* re, int steps );
    virtual float get();
    void set( float value /* 0..1 */ );

  protected :
    RotaryEncoder* re;
    int steps;
};

#endif
