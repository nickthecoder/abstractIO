#include "abstract_shift595.h"
#include "SPI.h"

Shift595::Shift595( int byteCount )
{
    this->buffer = (byte*) malloc( byteCount );
    this->byteCount = byteCount;
    for ( int i = 0; i < byteCount; i ++ ) {
        this->buffer[i] = 0;
    }
    this->bitOrder = LSBFIRST;
}

Shift595* Shift595::pins( int latchPin )
{
    this->latchPin = latchPin;
    this->clockPin = -1;
    this->dataPin = -1;

    pinMode( latchPin, OUTPUT );

    SPI.begin();
    
    return this;
}

Shift595* Shift595::pins( int latchPin, int clockPin, int dataPin )
{
    this->latchPin = latchPin;
    this->clockPin = clockPin;
    this->dataPin = dataPin;
    
    pinMode( latchPin, OUTPUT );
    pinMode( dataPin, OUTPUT );
    pinMode( clockPin, OUTPUT );
    
    return this;
}

#define WAIT delay(1);

void Shift595::update()
{
    digitalWrite( this->latchPin, LOW );

    if ( this->clockPin < 0 ) {
        // Use hardware SPI to shift the data
        for ( int i = 0; i < this->byteCount; i ++ ) {   
            SPI.transfer(this->buffer[i]);
        }
        
    } else {

        for ( int i = 0; i < this->byteCount; i ++ ) {   
            shiftOut( this->dataPin, this->clockPin, this->bitOrder, this->buffer[i] );
        }
    }
        
    // Move data from the shift register into the storage register
    digitalWrite( this->latchPin, HIGH );
}


