#include <abstractIO.h>

/*
Turns an LED on when a switch A is held down, when switch B is pressed and when switch C is released.

Connect a switches to ground and to pin 10, 11 and 12
Look at the on-board LED, or connect an LED in series with a resistor to pin 13.
*/


Output* output1;

Input* inputA;
Button* buttonB;
Button* buttonC;

void setup()
{
    Serial.begin( 9600 );
    
    output1 = new SimpleOutput( 13 );
    
    inputA = new SimpleInput( 10 );
    buttonB = (new SimpleInput( 11 ))->debounced()->button();
    buttonC = (new SimpleInput( 12 ))->debounced()->button();
}

void loop()
{

    // Hold down the first switch to light the LED
    output1->set( inputA->get() );
   
    // Press the 2nd switch to light the LED for a short period. Holding the switch down makes no difference.
    if ( buttonB->pressed() ) {
        Serial.println( "Button B pressed" );
        output1->set( true );
        delay( 200 );
    }

    // Press and then release the 3rd switch to light the LED for a short period. Holding the switch down makes no difference.
    if ( buttonC->released() ) {
        Serial.println( "Button C released" );
        output1->set( true );
        delay( 200 );
    }
}

