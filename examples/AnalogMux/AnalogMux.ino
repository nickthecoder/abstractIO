#include <Arduino.h>
#include <abstractIO.h>
#include <abstractShiftRegister.h>

// Here are a few different ways of creating an AnalogMux. Uncomment the one you want to play with...

// Simple example
//AnalogMux* analogMux = AddressSelector( 13, 12, 11 ).createAnalogMux( A0 );

// Complex example
// AnalogMux* analogMux = AddressSelector( 13, 12, 11 ).createAnalogMux( (new SimpleAnalogInput( A0 ))->ease( &easeInQuad ) );

// Reverse the values for all of the inputs
AnalogMux* analogMux = AddressSelector( 13, 12, 11 ).createAnalogMux( (new SimpleAnalogInput( A0 ))->clip( 1.0, 0.0 ) );

// Using a ComboSelector from abstractShiftRegister.h
//AnalogMux* analogMux = ComboSelector( new LatchedShiftRegister(13, 12, 11) ).createAnalogMux( A0 );

AnalogInput *i0 = analogMux->createInput( 0 );
AnalogInput *i1 = analogMux->createInput( 1 );
//AnalogInput *i1 = analogMux->createInput( 1 )->clip( 1.0, 0.0 ); // Reverse the direction of just this input.

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.print( "Input 0 :" ); Serial.println( i0->get() );
    Serial.print( "Input 1 :" ); Serial.println( i1->get() );
    Serial.println();
    delay( 1000 );
}

