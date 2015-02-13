#include "abstract_M74HC595.h"


M74HC595::M74HC595( int outputCount )
{
    this->buffer = (boolean*) malloc( outputCount * sizeof(boolean) );
    this->outputCount = outputCount;
    for ( int i = 0; i < outputCount; i ++ ) {
        this->buffer[i] = i % 2;
    }
}

#define WAIT delay(1);

void M74HC595::update()
{
    digitalWrite( this->latchPin, LOW );
    digitalWrite( this->clockPin, LOW );
    
    for ( int i = 0; i < outputCount; i ++ ) {
        digitalWrite( this->dataPin, this->buffer[i] );
        digitalWrite( this->clockPin, HIGH );
        digitalWrite( this->clockPin, LOW );
    }

    // Move data from the shift register into the storage register
    digitalWrite( this->latchPin, HIGH );
}


