/*
 * Copyright (c) 2015 Nick Robinson All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0 which accompanies this
 * distribution, and is available at http://www.gnu.org/licenses/gpl.html
*/

#ifndef abstractIO_h
#define abstractIO_h

#include <Arduino.h>

#define ABSTRACT_NOT_USED 255 // Indicates that this pin number is not set

// Simple macros to aid debugging. Unomment the following line to disable debugging.
#define ABSTRACT_DEBUG

#ifdef ABSTRACT_DEBUG

#define IO_DEBUG1( a ) Serial.println( a );
#define IO_DEBUG2( a, b ) Serial.print( a ); Serial.print( " " ); Serial.println( b );;
#define IO_DEBUG3( a, b, c ) Serial.print( a ); Serial.print( " " ); Serial.print( b ); Serial.print( " " ); Serial.println( c );
#define IO_DEBUG4( a, b, c, d ) Serial.print( a ); Serial.print( " " ); Serial.print( b ); Serial.print( " " ); Serial.print( c ); Serial.print( " " ); Serial.println( d );
#define IO_DEBUG5( a, b, c, d, e ) Serial.print( a ); Serial.print( " " ); Serial.print( b ); Serial.print( " " ); Serial.print( c ); Serial.print( " " ); Serial.print( d ); Serial.print( " " ); Serial.println( e );

#else

#define IO_DEBUG1( a )
#define IO_DEBUG2( a, b )
#define IO_DEBUG3( a, b, c )
#define IO_DEBUG4( a, b, c, d )
#define IO_DEBUG5( a, b, c, d, e )

#endif

class AbstractSerial {
  public :
    AbstractSerial( int baud );
};

// Here's a "contents" page, broken up into groups.
// The top of the group is typically a pure-virtual class (interface), with implementations below it.

class Input;
class SimpleInput;
class DebouncedInput;
class BinaryInput;
// See abstractRemote.h and abstractMux.h for other Input implementaions.

class Button;
class InputButton;
class CompoundButton;
// See abstractRemote.h for RemoteButton

class Output;
class SimpleOutput;
class BufferedOutput;

class AnalogInput;
class SimpleAnalogInput;
class ClippedAnalogInput;
class ScaledAnalogInput;
class EasedAnalogInput;
class AnalogMuxInput;

class PWMOutput;
class SimplePWMOutput;
class EasedPWMOutput;
class ScaledPWMOutput;

class Selector;
class AddressSelector;
class BooleanSelector;
// See abstractLineDecoder.h and abstractShiftRegister.h for other Selector implementaions.

class Ease;
class Linear;
class Jump;
class EaseInQuad;
class EaseInCubic;
class EaseInQuart;
class EaseOutQuad;
class EaseOutCubic;
class EaseOutQuart;

class Mux;
class AnalogMux;

/*
 * A digital input, returning true or false.
 * Abstract base class for all 'Input' classes.
 */
class Input
{
  public :
    
    // Read the state of the input
    virtual boolean get() = 0;

    // Create a debounced version of this input.
    Input* debounced();
    
    // Create a Button from this input.
    InputButton* button();
};

/*
 * A simple digital input directly accessed using digialRead on a given pin.
 * The 'trueReading' parameter of the constructor determins if LOW is true, or if HIGH is true.
 * 
 * For simple switches, consider omitting an external pull up/down resistor and enable the Arduino's internal pullup resistor by using :
 */
 // SimpleInput( the_pin_number, LOW, /*enable pullup*/ true );
class SimpleInput : public Input
{
  public :
    byte pin;
    byte trueReading;  // When is "true" returned, with a LOW or a HIGH reading?

  public :
    // Use this for a simple switch, using the built in pullup resistor.
    SimpleInput( int pin ) : SimpleInput( pin, LOW, true ) {};
    
    // trueReading : Should a LOW or HIGH reading return true from get()?
    SimpleInput( int pin, boolean trueReading /* HIGH or LOW */, boolean enablePullup = false );
        
    // Read the state of the input
    virtual boolean get();
};

/*
 * Wraps an input, and filters its signal to prevent spurious values caused by "bounce"
 * It does this by ignoring values until the input has shown a stable value for at least 50ms.
 * After thinking about this problem, I believe this class isn't really needed. The only reason
 * that key bounce is a problem, is when the signal is processed within the timescale of a bounce.
 * If a key bounce takes N millsseconds before the signal is stable, then bounce will never be a problem
 * if your event loop is longer than N milliseconds.
 * See function delayPeriod( int millis ).
 */
class DebouncedInput : public Input
{
  public :
    int debounceMillis;

  private :
    Input* wrapped;  // The un-debounced input - often a SimpleInput
    boolean previousReading; // The previous raw reading from digitalRead
    long stableTime;         // The time in millis when the button was last stable i.e. not fluctuating caused by button "bounce"
    boolean stableState;     // The state of the button when it was last stable (i.e. ignoring the debounce noise)
    
  public :
    DebouncedInput( Input* wrap, int debounceMillis = 50 );
    
    virtual boolean get();
};

/*
 * Delays for upto 'millis' milliseconds, but rather than a fixed delay, it delays based on the time since the last call to this function.
 * For example, if we call this 40 milliseconds after the previous call, with a value of millis = 100,
 * then the delay will be 60 milliseconds.
 * If the period between the calls > millis, then no delay occurs.
 * 
 * You can use this at the top of your main loop() method if you want to make sure your code doesn't run "too fast".
 * For example, if you read the state of buttons too fast, then "key bounce" can cause your code to register multiple
 * button presses. Put delayPeriod(50) in your loop(), and key bounce won't be a problem (assuming key bounce takes less time than
 * 50 milliseconds).
 */
extern void delayPeriod( int millis );

class RunPeriodically {
  private :
      unsigned long period;
      unsigned long prevMillis;
      void (*callback)(void);
      
  public :
    RunPeriodically( unsigned long millis, void (*callback)(void) );
    void run();
};

/*
 * Converts an AnalogInput into a (digital) Input.
 * This is useful for inputs from analog sources that you want to use as on/off states.
 * For example, a light dependent resistor used to determine if it is day or night.
 */
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


/*
 * While an Input only knows if a switch is currently on or off, a Button remembers its previous state, so that it
 * can detect when a button has just been pressed, and when it has just been released.
 * This is an interface (pure vitual class). The simplest concrete implementation is InputButton.
 */
class Button
{
  public :
    virtual boolean pressed() = 0;
    virtual boolean released() = 0;
    virtual boolean get() = 0; // Returns the current state (just like Input).
};

/*
 * While an Input only knows if a switch is currently on or off, a Button remembers its previous state, so that it
 * can detect when a button has just been pressed, and when it has just been released.
 * The method Input::button() is the easiest way to create an InputButton.
*/
class InputButton : public Button
{
  public :
    Input* input;
    
  private :
    boolean pressedDetected; // The previous value the last time pressed was called.
    boolean releasedDetected; // The previous value the last time released was called.
      
  public :
    InputButton( Input* input );
    
    virtual boolean pressed();
    virtual boolean released();
    virtual boolean get();
};

/*
 * Combines multiple buttons together, and get()/pressed()/released() will return true if any of them
 * return true.
 * This is useful if your project has different means of inputting the same information.
 * For example, a physical button, and also an infra red signal.
 */
class CompoundButton : public Button
{
  private :
    Button *buttonA;
    Button *buttonB;
    
    
  public :
    CompoundButton( Button *buttonA, Button *buttonB );
    
    virtual boolean get();
    virtual boolean pressed();
    virtual boolean released();
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
    byte *buffer;
    byte mask;
    
  public :
    BufferedOutput( boolean* buffer, int index );
    virtual void set( boolean value );
};

/*
 * Abstracts analog inputs. The application code should not need to know the details of how to read the analog input.
 * For example reading an analog input firectly from one of the analog pins, should be similar to reading a value
 * from a demultiplexer.
 * 
 * The value returned by method get() are in the range 0..1, and NOT the usual range of 0..1023 returned by Arduino's analog pins.
 * This is to hide (or abstract) away the details of the hardware, so that it is easy to change the hardware without ching our code.
 * For example, using a different microcontroller, or using an external ADC chip, the hardware may have a different range.
 * 
 * Also, by using a range of 0..1, we can do some nice tricks, such as using an "Ease" to pre-process the value before use.
 * For example, a linear potentiometer can appear as if it were logarithmic by using EaseInQuad.
 * 
 * Speed. For those worried about floats being slow (the Arduino does floating point maths in software) :
 * A division takes about 34 microseconds (http://forum.arduino.cc/index.php?topic=40901.0)
 * anlogRead takes about 100 microseconds (https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/)
 * Hopefully you can now work out if speed will be an issue in your project.
 * Using floats when reading analog inputs will never be a problem for the kinds of projects I create. YMMV.
 * However, using AnalogInput may not be appropriate if you need high-speed data logging from an analog source.
 */
class AnalogInput
{
  public :
    virtual float get() = 0; // All direct (unmapped) measurements should be in the range 0..1
    
    // Creates a new AnlogInput, by applying an Ease
    EasedAnalogInput* ease( Ease *ease );
    
    // Creates a new AnalogInput by clipping the input.
    ClippedAnalogInput* clip( float minimum, float maximum );
    
    // Creates a new AnalogInput, whose values are no longer in the range 0..1, but scaled to 0..whatever-you-want.
    // E.g. To control an RGB value, have three AnalogInputs, and scale them by 255.
    // This can also be used to adjust an AnalogInput whose values aren't within the usual range.
    ScaledAnalogInput* scale( float scale );
    
    // Converts an analog input into a digital (on/off) Input.
    BinaryInput* binary( float calibration = 0.5, boolean reversed = false );
};

/*
 * The simplest type of AnalogInput, whose value is taken directly from analogRead().
 * The result of get() is scaled down by 1023, so that it fits in the range 0..1 inclusive.
 * If your harware has a different range, then use scale() to adjust it.
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
 * An Input using a Mux, such as the 4051, which can multiplex 8 analog inputs to one analog input on the Arduino.
 */
class AnalogMuxInput : public AnalogInput
{
  protected :
    AnalogMux *mux;
    byte address;

  public :
      AnalogMuxInput( AnalogMux *mux, byte address );
      virtual float get();
};
/*
 * Sometimes an analog input doesn't extend over the full range of 0..1, and you want to adjust the values, so that they still return a
 * complete range of 0..1.
 * In which case, use ClippedAnalogInput, to chop off the low and/or high end, and normalise it back to 0..1.
 *
 * Note that you can reverse the low and high values, in which case the result will still be in the range 0..1, but will be backwards
 * compared to the input.
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
 * The raw signals will be hardware dependent, but these can be mapped to something
 * which makes sense to the application.
 * For example, a simple volume knob may have a raw range of 0..1, but the application
 * may want to deal with a range 0..10 (or 0..11 if you want to turn your amp up to eleven).
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

/*
 * Create an abstract layer, so that outputting PWM signals is simple for your application regardless of the details.
 * This may not sound useful if you only ever use PWM chips directly on the Arduino's ATMega chip, but what happens
 * when you need one more PWM channel and use a chip such as WS2803D or TLC5940?
 * PWMOutput will allow your application code to remain unchanged, and only the setup routine needs to know about the details.
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


class Selector {

  public :
    virtual void select( byte address ) = 0;
    
    Mux* createMux( byte inputPin ); // A more fluent API for creating a Mux whose address is selected using this Selector.
    Mux* createMux( Input *input ); // A more fluent API for creating a Mux whose address is selected using this Selector.
    
    AnalogMux* createAnalogMux( byte inputPin ); // A more fluent API for createing an AnalogMux.
    AnalogMux* createAnalogMux( AnalogInput *analogInput ); // A more fluent API for createing an AnalogMux.
};

/*
 * A selector using n bits to select the address. Each address bit is a pin on the Arduino.
 * This can be used as the input to a line decoder, such as 74xx138, which takes an address,
 * and converts it into a set of "enable" lines,  where one is LOW, and the rest are HIGH.
 * 
 * Note, if using more than one 74xx138, then more than 3 address bits can be used. For example,
 * for upto 16 chip select lines, use 4 address bits like so :
 *    byte* addressPins = [ 13, 12, 11, 10 ];
 *    LineDecoder( 4, addressPins );
 * Now connect pins 13, 12, and 11 to the address pins of both 74xx138, then connect pin 10 to
 * G2A (pin 4) of the first 138 chip, and also to G1 (pin 6) of the second 138 chip.
 * This combination ensures that only one of the 138 chips to be enabled at any time.
 * 
 * A similar trick can be done with a 5th address bit (using G2B (pin 5) of the 138 chip),
 * allowing up to 32 chip select lines.
 * 
 * However, this trick uses increasing number of precious pins on the Arduino.
 * So for large numbers of addresses, consider using ShiftRegisterSelector, which uses only 3 control pins
 * (at the expense of being slower, as the data is clocked into the  shift register sequentially).
 *
 * It can also be used as the address on multiplexers, such as the 4051 chip.
 * 
*/
class AddressSelector : public Selector
{
  protected :
    byte pinCount;
    byte* addressPins;

  public :
    // The number of address pins, and their pin numbers.
    // Note, the LOW bit of the address must be first in the array.
    AddressSelector( byte pinCount, byte* addressPins );
    AddressSelector( byte a0, byte a1, byte a2 ); // A convienence, as many chips have three address lines

    void select( byte address );
};

/*
 * Uses a single output pin to select one of two chips.
 * Use an inverter on the chips select pin, so that one of the chips is enabled at a time.
 */
class BooleanSelector : public Selector {

  private :
    byte selectPin;
    
  public :
    BooleanSelector( byte selectPin );
    virtual void select( byte address );
};

/*
 * Converts a number in the range 0..1 to another number, usually also in the range 0..1
 * When used with EasedAnalogInput they can change the distribution of values, in much the same way that a logarithmic potentiometer has
 * a different distribution to a linear pot.
 * However, Ease can be used in other scenarios too, for example it can change the acceleration of an animated object,
 * or the rate of colour change when using PWM to change the intensity of LED lights.
 */
class Ease {
  public :
    virtual float ease( float from ) = 0;
};

/*
 * A "do nothing" ease. Returns the same number as the input.
 * Output = input
 */
class Linear : public Ease {
  public :
    virtual float ease( float from );
};

/*
 * If the input is below 0.5, then 0 is returned. Otherwise 1 is returned.
 * Output = input < 0.5 ? 0 : 1
 */
class Jump : public Ease {
  public :
    virtual float ease( float from );
};

/*
 * Output = input * input
 * When the input is close to 0, changes make little difference.
 * When the input is close to 1, changes are exagerated.
 */
class EaseInQuad : public Ease {
  public :
    virtual float ease( float from );
};

/*
 * Output = input * input * input
 * Like EaseInQuad, but even more exagerated.
 */
class EaseInCubic : public Ease {
  public :
    virtual float ease( float from );
};

/*
Ouput = input * input * input * input
Like EaseInQuad, but extremely exagerated.
*/
class EaseInQuart : public Ease {
  public :
    virtual float ease( float from );
};

/*

*/
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

/*
Instances of each of the Ease classes.
*/
extern Linear linear;
extern Jump jump;
extern EaseInQuad easeInQuad;
extern EaseInCubic easeInCubic;
extern EaseInQuart easeInQuart;
extern EaseOutQuad easeOutQuad;
extern EaseOutCubic easeOutCubic;
extern EaseOutQuart easeOutQuart;


class Mux
{
  protected :
    Selector *selector;
    Input *input;

  public :
    Mux( Selector *selector, byte inputPin );
    Mux( Selector *selector, Input *input );

    /*
     * Creates an Input for a single multiplexed input.
     */
    Input* createInput( byte address );
    
    boolean get( byte address );
};

/*
 * An Input which is multiplexed. For example a 4051 can multiplex 8 input channels into one input pin
 * on the Arduino.
 */
class MuxInput : public Input
{
  protected :
    Mux *mux;
    byte address;

  public :
      MuxInput( Mux *mux, byte address );
      virtual boolean get();
};

class AnalogMux
{
  protected :
    Selector *selector;
    AnalogInput *input;

  public :
      
    /*
     * Often, input will be a SimpleAnalogInput, however, you could apply wrappers (such as EasedAnalogInput)
     * so that all multiplexed inputs are wrapped in the same way.
     */ 
    AnalogMux( Selector *selector, AnalogInput *input );
    
    /*
     * Creates an AnalogInput for a single multiplexed analog input.
     */
    AnalogInput* createInput( byte address );
    
    /*
     * Gets the analog value for one of the multiplexed inputs.
     */
    float get( byte address );
};

#endif

