
/* 
Does the same as the SimpleInputOutput example, but uses an MCP23017

Turns an LED on when switch A is held down, when switch B is pressed and when switch C is released.

Connect I2C clock (A5) to pin 12 of the MCP23017 and I2C data (A4) to pin 13 of the MCP23017

Connect switches to ground and to data lines 2 and 3 (pins 23 and 24 or the MCP23017)
Connect LEDs from +5V to data line 3 and 4 (pins 25 and 26) of the MCP23017 via suitable resistors.

Use the "default" I2C address by tieing pins 15,16 and 17 to ground.
Tie the reset (pin 18) of the MCP23017 to 5V (it's active LOW).
Power the MCP23017 by connecting pin 9 to ground and pin 11 to 5V.
*/

#include <Wire.h>
#include <abstractIO.h>
#include <abstractMCP23017.cpp.h>

MCP23017* ioExpander = new MCP23017();

// NOTE. We cannot create the inputs and outputs here, because to do so we need to change the pin mode via I2C, and
// it seems that Wire doesn't work until setup().
// So we can only define the inputs and outputs here, and create them in setup().
Output* output1;
Output* output2;

Input* inputA;

Button* buttonB;
Button* buttonC;

void setup()
{
    Serial.begin( 9600 );
    output1 = ioExpander->createOutput( 0 );
    output2 = ioExpander->createOutput( 1 );

    inputA = ioExpander->createInput( 2 );

    buttonB = ioExpander->createInput( 3, LOW )->button();
    buttonC = ioExpander->createInput( 4, LOW )->button();    

    Serial.println( "setup complete" );
}

// The rest of the code is identical to the SimpleInputOutput example.
void loop()
{    
    // Hold down the first switch to light the LED
    // Note that inputA is not a Button, and therefore doesn't have the pressed() and released() methods.
    output1->set( inputA->get() );
    output2->set( inputA->get() );
        
    // Flash LED#1 once when the 2nd button is pressed. Holding the switch down makes no difference.
    if ( buttonB->pressed() ) {
        Serial.println( "Button B pressed" );
        output1->set( true );
        delay( 200 );
    }
    // Flash LED#1 twice when the 2nd button is released.
    if ( buttonB->released() ) {
        Serial.println( "Button B released" );
        output1->set( true );
        delay( 50 );
        output1->set( false );
        delay( 50 );
        output1->set( true );
        delay( 50 );
    }

    // Flash LED#2 once when the 3rd button is released. Holding the switch down makes no difference.
    if ( buttonC->pressed() ) {
        Serial.println( "Button C pressed" );
        output2->set( true );
        delay( 200 );
    }

    // Flash LED#2 twice when the 3rd button is released.
    if ( buttonC->released() ) {
        Serial.println( "Button C released" );
        output2->set( true );
        delay( 50 );
        output2->set( false );
        delay( 50 );
        output2->set( true );
        delay( 50 );
    } 
}

