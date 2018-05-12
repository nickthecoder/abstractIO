#include <abstractIO.h>

/*
Demonstates how to use Eases with PWM pins to give a nicer fade.
Fade 3 LEDs in and out continuously.

You would imagine that a linear fade from 0 to 255 would give a nice even fade, but for my LEDs it isn't.
There is very little difference between values over 128, so the light appears fully bright for most much of the cycle.

Using EaseIn, gives a much nicer fade. The EaseOut give an even sharper (worse) fade.
*/


PWMOutput* regular;
PWMOutput* easeIn;
PWMOutput* easeOut;

void setup()
{
    Serial.begin( 9600 );

    regular = new SimplePWMOutput( 9 );
    easeIn = (new SimplePWMOutput( 10 ))->ease( &easeInQuart ); // Try EaseInCubic and EaseInQuad too.
    easeOut = (new SimplePWMOutput( 11 ))->ease( &easeOutQuart ); // Try EaseOutCubic and EaseOutQuad too.
}

void loop()
{
    // Notice how all the outputs are set identically, but the visuals are different.
    for ( float i = 0; i < 1; i += 0.01 ) {
        regular->set( i );
        easeIn->set( i );
        easeOut->set( i );
        delay( 10 );
    }
    for ( float i = 1; i > 0; i -= 0.01 ) {
        regular->set( i );
        easeIn->set( i );
        easeOut->set( i );
        delay( 10 );
    }
}

