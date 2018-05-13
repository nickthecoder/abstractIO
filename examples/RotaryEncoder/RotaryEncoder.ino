#include <abstractRotaryEncoder.h>

RotaryEncoder* re = new SimpleRotaryEncoder( 4,5 );

int oldValue = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if ( oldValue != re->get() ) {
        oldValue = re->get();
        Serial.println( oldValue );
    }
}

