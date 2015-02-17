#include "abstract_shift595.h"

Shift595::Shift595( int latchPin, int clockPin, int dataPin, int byteCount )
{
    this->latchPin = latchPin;
    this->clockPin = clockPin;
    this->dataPin = dataPin;
    
    pinMode( latchPin, OUTPUT );
    pinMode( dataPin, OUTPUT );
    pinMode( clockPin, OUTPUT );
    
    this->buffer = (byte*) malloc( byteCount );
    this->byteCount = byteCount;
    for ( int i = 0; i < byteCount; i ++ ) {
        this->buffer[i] = 0;
    }
    this->bitOrder = MSBFIRST;
}

void Shift595::update()
{
    digitalWrite( this->latchPin, LOW );

    for ( int i = 0; i < this->byteCount; i ++ ) {   
        shiftOut( this->dataPin, this->clockPin, this->bitOrder, this->buffer[i] );
    }
        
    // Move data from the shift register into the storage register
    digitalWrite( this->latchPin, HIGH );
}




Shift595Selector::Shift595Selector( int latchPin, int clockPin, int dataPin, int addressLines )
  : Shift164Selector( clockPin, dataPin, addressLines, false )
{
    this->latchPin = latchPin;
    pinMode( latchPin, OUTPUT );

    digitalWrite( latchPin, LOW );
    this->initialise();
    digitalWrite( latchPin, HIGH );
}

void Shift595Selector::select( int address )
{   
    if ( address == this->previousAddress ) {
        return;
    }
        
    digitalWrite( latchPin, LOW );
    Shift164Selector::select( address );
    digitalWrite( latchPin, HIGH );
}



