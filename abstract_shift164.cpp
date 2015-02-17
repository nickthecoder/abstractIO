#include "abstract_shift164.h"

Shift164Selector::Shift164Selector( byte clockPin, byte dataPin, byte addressLines, boolean initialise )
{
    DVAL1( "164  lines", addressLines );

    this->clockPin = clockPin;
    this->dataPin = dataPin;
    this->addressLines = addressLines;

    pinMode( clockPin, OUTPUT );
    pinMode( dataPin, OUTPUT );

    if ( initialise ) {
        this->initialise();
    }
}

void Shift164Selector::initialise()
{
    // Ensure the outputs are in a valid state. Set address to 0
    digitalWrite( dataPin, LOW );
    this->shift( this->addressLines - 1 );
    digitalWrite( dataPin, HIGH );
    this->shift( 1 );
    this->previousAddress = 0;
}

void Shift164Selector::select( int address )
{
    if ( address == this->previousAddress ) {
        return;
    }
        
    digitalWrite( this->dataPin, LOW );
                
    if ( address > this->previousAddress ) {
        // We are currently LESS than the required address, so just shift by the difference.

        this->shift( address - this->previousAddress );

    } else {
        
        // Hardest case. We are currently AFTER the required. So we need to shift the old "1"
        // out of the way, and move a new "1" into place.
        // So we need some 0s shifted in, then a new 1, then some more 0s.

        int shiftOut = this->addressLines - this->previousAddress; // Number of shifts to remove the old 

        // We will shift address + 1 lines later, so just do shiftOut - (address + 1) now.
        this->shift( shiftOut - address - 1 );
        digitalWrite( this->dataPin, HIGH );
        this->shift( 1 );
        digitalWrite( this->dataPin, LOW );
        this->shift( address );
    }

    this->previousAddress = address;
}

void Shift164Selector::shift( int n )
{
    for ( int i = 0; i < n; i ++ ) {
        digitalWrite( this->clockPin, LOW );
        digitalWrite( this->clockPin, HIGH );
    }
}



