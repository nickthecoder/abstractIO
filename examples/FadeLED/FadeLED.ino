#include <abstractIO.h>
#include <abstractRotaryEncoder.h>

/*
Demonstates how to use Eases with PWM pins to give a nice fade.
Adjusts the brightness of 2 LEDs via potentiometers.
Another LED is controlled via a rotary encoder.

You would imagine that a linear fade from 0 to 255 would give a nice even fade, but for my LEDs it does't.
There is very little difference between values over 128, so the light appears fully bright for most of the range.

Using EaseIn, gives a much nicer fade.
*/

PWMOutput* pwm1 = new SimplePWMOutput( 3 ); // No ease. The PWM value is linear with respect to the analog input.
PWMOutput* pwm2 = (new SimplePWMOutput( 6 ))->ease( &easeInQuart );
PWMOutput* pwm3 = (new SimplePWMOutput( 9 ))->ease( &easeInQuart );
// Try EaseInCubic and EaseInQuad. EaseOutCubic gives the opposite effect.

AnalogInput* input1 = new SimpleAnalogInput( A1 );
AnalogInput* input2 = new SimpleAnalogInput( A0 );
REAnalogInput* input3 = (new SimpleRotaryEncoder( 4, 5 ))->createAnalogInput( 50 );

void setup()
{
  input3->set( 0.5 ); // Start the rotary encoder at the mid-way point.
}

void loop()
{
    pwm1->set( input1->get() );
    pwm2->set( input2->get() );
    pwm3->set( input3->get() );
}

