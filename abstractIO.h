/*
 * Copyright (c) 2015 Nick Robinson All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0 which accompanies this
 * distribution, and is available at http://www.gnu.org/licenses/gpl.html
*/

#ifndef abstractIO_h
#define abstractIO_h

#include <Arduino.h>

// Declared lower down.
class Ease;
class Button;
class SimpleInput;

/*
A digital input, returning true or false.
Abstract base class for all 'Input' classes.
*/
class Input
{
  public :
    
    // Read the state of the input
    virtual boolean get() = 0;

    // Create a debounced version of this input.
    Input* debounced();
    
    // Create a button from this input
    Button* button();
};

/*
A simple digital input directly accessed using digialRead on a given pin.
The 'trueReading' parameter of the constructor determins if LOW is true, or if HIGH is true.
For simple switches, do NOT include a pull up/down resistor, and the defaults will use LOW as true,
and enable the Arduino's internal pullup resistor.
*/
class SimpleInput : public Input
{
  public :
    int pin;
    byte trueReading;  // When is "true" returned, with a LOW or a HIGH reading?

  public :
    // Use this for a simple switch, using the built in pullup resistor.
    SimpleInput( int pin ) : SimpleInput( pin, LOW, true ) {};
    
    // trueReading : Should a LOW or HIGH reading return true from get()?
    SimpleInput( int pin, byte trueReading, boolean enablePullup = false );
        
    // Read the state of the input
    virtual boolean get();
};

/*
Wraps an input, and filters its signal prevent spurious values caused by "bounce".
It does this by ignoring values until the input has shown a stable value for at least 50ms.
*/
class DebouncedInput : public Input
{
  public :
    int debounceMillis;

  private :
    Input* wrapped;   // The un-debounced input - often a SimpleInput
    boolean previousReading; // The previous raw reading from digitalRead
    long stableTime;         // The time in millis when the button was last stable i.e. not fluctuating caused by button "bounce"
    boolean stableState;     // The state of the button when it was last stable (i.e. ignoring the debounce noise)
    
  public :
    DebouncedInput( Input* wrap, int debounceMillis = 50 );
    
    virtual boolean get();
};


/*
Combines multiple inputs together, and get() will return true if any of them return true.
This is useful if you project has different means of inputting the same information.
For example, a physical button, and also an infra red signal.
*/
class CompoundInput : public Input
{
  private :
    Input **inputs; // An array of pointers to Input. Why can't I use Input *inputs[]; ?
    byte count;
    
  public :
    CompoundInput( Input **inputs, byte count );
    
    virtual boolean get();
};

/*
  You often want to know if a switch has been pressed, not if the switch is currently held down.
  i.e. you only want it to return true ONCE per press, rather than every time through the loop
  until the switch is released.
  
  Button takes an Input, and keeps track of its state.
*/
class Button
{
  public :
    Input* input;
    
  private :
    boolean previousState; // The previous value the last time pressed was called.
      
  public :
    Button( Input* input );
    boolean pressed();
    boolean released();
    boolean get(); // Returns the current state (just like Input).
};

class Output {
  public :
    virtual void set( boolean value ) = 0;
};

class SimpleOutput : public Output
{
  public :
    int pin;
    boolean lowValue;
    
  public :
    SimpleOutput( int pin, boolean lowVale = false );
    virtual void set( boolean value );    
};

class BufferedOutput : public Output
{
  public :
    boolean *buffer;
    
  public :
    BufferedOutput( boolean* buffer );
    virtual void set( boolean value );
};

class EasedAnalogInput;
class ClippedAnalogInput;
class ScaledAnalogInput;
class BinaryInput;

/*
Abstracts analog inputs. The application code should not need to know the details of how to read the analog input
(which may not be one of the directly readable analog pins).

NOTE, that direct analog inputs should return values in the range 0..1.
This is arguably nicer than an arbitrary integer range (0..1024 for an Arduino). A range of 0..1 also plays nicely with other things, such as the "Ease" class.
It is also necessary to give a consistent interface with other analog sources (e.g. external ADCs which may have a different range.
*/
class AnalogInput
{
  public :
    virtual float get() = 0; // All direct (unmapped) measurements should be in the range 0..1
    
    EasedAnalogInput* ease( Ease *ease );
    ClippedAnalogInput* clip( float minimum, float maximum );
    ScaledAnalogInput* scale( float scale );
    BinaryInput* binary( float calibration = 0.5, boolean reversed = false );
};

/*
Note that the output of get is in the range 0..1 (inclusive), which is NOT the raw output from analogRead.
It does this because we want a consistent interface when mixing and matching different inputs, which may
include external analog to digital converts, with different range's to the Arduino's analag pins.
*/
class SimpleAnalogInput : public AnalogInput
{
  public :
    int pin;
    
  public :
    SimpleAnalogInput( int pin );
    
    virtual float get();
};


/*
Sometimes an analog input doesn't extend over the full range of 0..1, and you want to adjust the values, so that they still return a complete range of 0..1.
In which case, use ClippedAnalogInput, to chop off the low and/or high end, and normalise it back to 0..1.

Note that you can reverse the low and high values, in which case the result will still be in the range 0..1, but will be backwards compared to the input.
*/
class ClippedAnalogInput : public AnalogInput
{
  private :
    AnalogInput* wrapped;

  public:
    float minimum;
    float maximum;

  public :
    ClippedAnalogInput( AnalogInput* wrap, float low, float high );
    
    virtual float get();
  
};

/*
The raw signals will be hardware dependent, but these can be mapped to something
which makes sense to the application.
For example, a simple volume knob may have a raw range of 0..1, but the application
may want to deal with a range 0..10 (or 0..11 if you want to turn your amp up to eleven).
*/
class ScaledAnalogInput : public AnalogInput
{

  private :
    AnalogInput* wrapped;
    float scale;

  public :
    ScaledAnalogInput( AnalogInput* wrap, float scale );
    
    virtual float get();
};

class EasedAnalogInput : public AnalogInput
{
  public :
    Ease* ease;
    
  private :
    AnalogInput* wrapped;
    
  public :
    EasedAnalogInput( AnalogInput* wrap, Ease* ease );
    
    virtual float get();
};

class BinaryInput : public Input
{
  private :
    AnalogInput *wrapped;
    float calibration;
    boolean reversed;
    
  public :
    BinaryInput( AnalogInput* wrap, float calibration = 0.5, boolean reversed = false );
    virtual boolean get();
};

class ScaledPWMOutput;
class EasedPWMOutput;

/*
Create an abstract layer, so that outputting PWM signals is simple for your application regardless of the details.
This may not sound useful if you only ever use PWM chips directly on the Arduino's ATMega chip, but what happens
when you need one more PWM channel and use a chip such as WS2803D or TLC5940?
PWMOutput will allow your application code to remain unchanged, and only the setup routine needs to know about the details.
*/
class PWMOutput 
{
  public :
    virtual void set( float value ); // Range 0..1 inclusive
    ScaledPWMOutput* scale( float scale );
    EasedPWMOutput* ease( Ease* ease );
};

class SimplePWMOutput : public PWMOutput
{
  public :
    byte pin;
    
  public :
    SimplePWMOutput( int pin );
    
    virtual void set( float value ); // Range 0..1 inclusive
};

class EasedPWMOutput : public PWMOutput
{
  private :
    PWMOutput* wrapped;
    
  public :
    Ease* ease;
    
  public :
    EasedPWMOutput( PWMOutput* wrap, Ease* ease );
    
    virtual void set( float value );
};


class ScaledPWMOutput : public PWMOutput
{
  private :
    PWMOutput* wrapped;
    float scale;
    
  public :
    ScaledPWMOutput( PWMOutput* wrap, float maximum );
    
    virtual void set( float value );
};

/*
Converts a number in the range 0..1 to another number, usually also in the range 0..1
When used with EasedAnalogInput they can change the distribution of values, in much the same way that a logarithmic potentiometer has
a different distribution to a linear pot.
However, Ease can be used in other scenarios too, for example it can change the acceleration of an animated object, or the rate of colour change
when using PWM to change the intensity of LED lights.
*/
class Ease {
  public :
    virtual float ease( float from ) = 0;
};


class Linear : public Ease {
  public :
    virtual float ease( float from );
};

class Jump : public Ease {
  public :
    virtual float ease( float from );
};

class EaseInQuad : public Ease {
  public :
    virtual float ease( float from );
};

class EaseInCubic : public Ease {
  public :
    virtual float ease( float from );
};

class EaseInQuart : public Ease {
  public :
    virtual float ease( float from );
};

class EaseOutQuad : public Ease {
  public :
    virtual float ease( float from );
};

class EaseOutCubic : public Ease {
  public :
    virtual float ease( float from );
};

class EaseOutQuart : public Ease {
  public :
    virtual float ease( float from );
};

extern Linear linear;
extern Jump jump;
extern EaseInQuad easeInQuad;
extern EaseInCubic easeInCubic;
extern EaseInQuart easeInQuart;
extern EaseOutQuad easeOutQuad;
extern EaseOutCubic easeOutCubic;
extern EaseOutQuart easeOutQuart;

#endif

