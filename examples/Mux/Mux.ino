#include <abstractIO.h>

Mux* mux = AddressSelector( 13, 12, 11 ).createMux( 10 );
Input *i0 = mux->createInput( 0 );
Input *i1 = mux->createInput( 1 );
// etc.

Button *b2 = mux->createInput( 2 )->button();

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (i0->get()) {
        Serial.println( "Input 0 set" );
    }
    if (i1->get()) {
        Serial.println( "Input 1 set" );
    }
    
    if (b2->pressed()) {
      Serial.println( "Button 2 pressed" );
    }
    if (b2->released()) {
      Serial.println( "Button 2 released" );
    }
}

