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
    //Serial.print( "Latch Low " ); Serial.println( latchPin );
    digitalWrite( this->latchPin, LOW );

    //Serial.print( "Clock pin " ); Serial.println( clockPin );

    for ( int i = 0; i < this->byteCount; i ++ ) {   
        //Serial.print( "ShiftOut " ); Serial.println( buffer[i] );
        shiftOut( this->dataPin, this->clockPin, this->bitOrder, this->buffer[i] );
    }
        
    // Move data from the shift register into the storage register
    digitalWrite( this->latchPin, HIGH );
    //Serial.print( "Latch High " ); Serial.println( latchPin );
}


