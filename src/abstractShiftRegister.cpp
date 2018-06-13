#include <abstractShiftRegister.h>

// SHIFT REGISTER

ShiftRegister::ShiftRegister( byte dataPin, byte clockPin, boolean order )
{
    this->dataPin = dataPin;
    this->clockPin = clockPin;
    this->order = order;
    
    digitalWrite( clockPin, LOW );
    digitalWrite( dataPin, LOW );
    pinMode( dataPin, OUTPUT );
    pinMode( clockPin, OUTPUT );
}

void ShiftRegister::output( byte value ) {
    shiftOut( this->dataPin, this->clockPin, this->order, value );
    this->latchOutput();
}

void ShiftRegister::output( byte first, byte second ) {
    shiftOut( this->dataPin, this->clockPin, this->order, first );
    shiftOut( this->dataPin, this->clockPin, this->order, second );
    this->latchOutput();
}

void ShiftRegister::output( byte byteCount, byte *values )
{
    for ( byte i = 0; i < byteCount; i ++ ) {
        IO_DEBUG5("shiftOut", this->dataPin, this->clockPin, this->order, values[i] );
        shiftOut( this->dataPin, this->clockPin, this->order, values[i] );
    }
    this->latchOutput();
}

void ShiftRegister::shift( boolean value, byte n )
{
    digitalWrite( dataPin, value );
    digitalWrite( clockPin, LOW );
    for (byte i = 0; i < n; i ++ ) {
        digitalWrite( clockPin, HIGH );
        digitalWrite( clockPin, LOW );
    }
}

void ShiftRegister::latchOutput()
{
    // Does nothing
}

BufferedShiftRegister* ShiftRegister::buffer( byte byteCount ) {
    return new BufferedShiftRegister( this, byteCount );
}

// LATCHED SHIFT REGISTER

LatchedShiftRegister::LatchedShiftRegister( byte dataPin, byte clockPin, byte latchPin, boolean order )
    : ShiftRegister( dataPin, clockPin, order )
{
    this->latchPin = latchPin;
    
    digitalWrite( latchPin, LOW ); // Disable the output latch initially, as the data hasn't be set yet.
    pinMode( latchPin, OUTPUT );
}

void LatchedShiftRegister::latchOutput()
{
    digitalWrite( latchPin, HIGH );
    digitalWrite( latchPin, LOW );
}

// BUFFERED SHIFT REGISTER

BufferedShiftRegister::BufferedShiftRegister( ShiftRegister *shiftRegister, byte byteCount )
{
    this->shiftRegister = shiftRegister;
    this->byteCount = byteCount;
    
    this->buffer = (byte*) malloc( byteCount );
    for ( int i = 0; i < byteCount; i ++ ) {
        this->buffer[i] = 0;
    }
}

void BufferedShiftRegister::set( byte index, boolean value )
{
    byte *val = this->buffer + (index >> 4);
    byte mask = 1 << (index %8);
    
    *val |= mask;
    if ( !value ) {
        *val ^= mask;
    }
}

void BufferedShiftRegister::update()
{
    this->shiftRegister->output( this->byteCount, this->buffer );
}


BufferedOutput** BufferedShiftRegister::createOutputs()
{
    BufferedOutput** result = (BufferedOutput**) malloc( sizeof(BufferedOutput*) * this->byteCount * 8 );
    for (int i = 0; i < this->byteCount; i ++ ) {
        for (int j = 0; j < 8; j ++ ) {
            result[i * 8 + j] = new BufferedOutput( this->buffer + i, j );
        }
    }
    return result;
}

// SHIFT REGISTER SELECTOR

ShiftRegisterSelector::ShiftRegisterSelector( ShiftRegister *shiftRegister, byte addresses, boolean activeHighLow )
{
    this->shiftRegister = shiftRegister;
    this->addresses = addresses;
    this->activeHighLow = activeHighLow;

    this->previousAddress = 1;
    select( 0 );
}

void ShiftRegisterSelector::select( byte address )
{
    if ( address == this->previousAddress ) {
        return;
    }
                
    if ( address > this->previousAddress ) {
        // We are currently LESS than the required address, so just shift by the difference.

        this->shiftRegister->shift( ! this->activeHighLow, address - this->previousAddress );
        this->shiftRegister->latchOutput();

    } else {
        // We are currently AFTER the required.
        
        // NOTE. it is important that there are never two "1s" at the shift register's outputs. We don't want to select multiple
        // chips at the same time. So shift the 1 out of the way first.
        this->shiftRegister->shift( ! this->activeHighLow, this->addresses - this->previousAddress );

        // Put the ACTIVE at the front.
        this->shiftRegister->shift( this->activeHighLow, 1 );
        // Now move it along to the correct place (adding extra INACTIVEs).
        this->shiftRegister->shift( ! this->activeHighLow, address );
        this->shiftRegister->latchOutput();
    }

    this->previousAddress = address;
}


// COMBO SELECTOR

ComboSelector::ComboSelector( ShiftRegister *shiftRegister, boolean activeHighLow )
{
    this->shiftRegister = shiftRegister;
    this->activeHighLow = activeHighLow;
    select( 0 );
}

void ComboSelector::select( byte address )
{
    // Take the first three bytes of the address and use them as the least significant bits of the output.
    // The remaining 5 bits are used as chip-selector lines (i.e. ONE of them is LOW, and the others are HIGH).
    byte chipSelect = 1 << (address >> 3);
    byte value = (address & 7) | ( (activeHighLow ? chipSelect : ~chipSelect) << 3);
    this->shiftRegister->output( value );
}

// END
