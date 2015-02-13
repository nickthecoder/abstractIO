#include <abstractIO.h>

/*
Take analog readings from A0, and show them in the serial console.
Use the same analog input, but clip it so that part of the range is ignored.

If we have a photo resistor (which is 50K when dark) in series with a 50K resistor,
then the lower half of the analog range is never used.

           PhotoR     47K
 +5    o---/\/\/--.--/\/\/---o GND
                  |
 Pin 0 o-----------
 
So, the clipped analog input will give 0 for black and 1 for very bright, which is much more useful than the unclipped one.

Finally, we can make a regular digital input, and then treat is like a regular switch.
*/


AnalogInput* raw;
AnalogInput* clipped;

Button* button;

Button* tripwire;

void setup()
{
    Serial.begin( 9600 );

    raw = new SimpleAnalogInput( A0 );
    clipped = raw->clip( 0.5, 1 );
    
    // Make a virtual button from the light sensor. Cover it to activate the button.
    // We use the default calibration of 0.5.
    button = clipped->binary()->button();
    
    // Lets make one more "button", using the same input. This time using a much higher calibration value, and
    // reversing the logic (so that a bright light is false, and a dull (low) value is true.
    tripwire = clipped->binary( 0.9, true )->button();
}

void loop()
{
    Serial.print( "Raw : " ); Serial.print( raw->get() ); Serial.print( " clipped : " ); Serial.println( clipped->get() );
    if ( button->pressed() ) {
        Serial.println( "(pressed) Covered" );
    }  
    if ( button->released() ) {
        Serial.println( "(released) Uncovered" );
    }  

    if ( tripwire->pressed() ) {
        Serial.println( "(pressed) Tripwire Armed" );
    }
    if ( tripwire->released() ) {
        Serial.println( "(released) Sound the alarms, the trip wire has been activated!" );
    }
    
    // Slow things down, so the console isn't flooded with messages.
    delay( 200 );
}

