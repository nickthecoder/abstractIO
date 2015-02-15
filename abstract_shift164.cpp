#include "abstract_shift164.h"

Shift164Selector::Shift164Selector( byte clockPin, byte dataPin, byte chipCount )
{
    this->clockPin = clockPin;
    this->dataPin = dataPin;
    this->chipCount = chipCount;
}

void Shift164Selector::select( int address )
{
    // Note, this is not speed optimised. It would be possible to keep track of the previous state,
    // and in many cases, perform less shifts (especiall if the chips are selected in sequence).
    
    for ( int i = 0; i < chipCount; i ++ ) {
        digitalWrite( this->clockPin, LOW );
        digitalWrite( this->dataPin, i == address ? HIGH : LOW );
        digitalWrite( this->clockPin, HIGH );
    }
}


