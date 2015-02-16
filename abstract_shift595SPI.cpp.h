// See abstract_shift595SPI.h for why this has a weird .cpp.h suffix.

Shift595SPI::Shift595SPI( int latchPin, int byteCount )
{
    this->latchPin = latchPin;
    pinMode( latchPin, OUTPUT );
        
    SPI.begin();
    
    this->buffer = (byte*) malloc( byteCount );
    this->byteCount = byteCount;
    for ( int i = 0; i < byteCount; i ++ ) {
        this->buffer[i] = 0;
    }
    this->bitOrder = MSBFIRST;
}

void Shift595SPI::update()
{
    //Serial.print( "Latch Low " ); Serial.println( latchPin );
    digitalWrite( this->latchPin, LOW );

    //Serial.print( "Clock pin " ); Serial.println( clockPin );
    // Use hardware SPI to shift the data
    for ( int i = 0; i < this->byteCount; i ++ ) {   
        //Serial.print( "SPI transfer " ); Serial.println( buffer[i] );
        SPI.transfer(this->buffer[i]);
    }
        
    // Move data from the shift register into the storage register
    digitalWrite( this->latchPin, HIGH );
    //Serial.print( "Latch High " ); Serial.println( latchPin );
}

