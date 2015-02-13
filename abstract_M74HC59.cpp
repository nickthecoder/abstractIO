#include "abstract_M74HC59.h"


M74HC59::M74HC59( int outputCount )
{
    this->buffer = (boolean*) malloc( outputCount * sizeof(boolean) );
    this->outputCount = outputCount;
    for ( int i = 0; i < outputCount; i ++ ) {
        this->buffer[i] = i % 2;
    }
}

#define WAIT delay(1);

void M74HC59::update()
{
    digitalWrite( this->storageClockPin, LOW );
    digitalWrite( this->shiftClockPin, LOW );
    
    for ( int i = 0; i < outputCount; i ++ ) {
        digitalWrite( this->dataPin, this->buffer[i] );
        digitalWrite( this->shiftClockPin, HIGH );
        digitalWrite( this->shiftClockPin, LOW );
    }

    // Move data from the shift register into the storage register
    digitalWrite( this->storageClockPin, HIGH );
}


