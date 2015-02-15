#include "abstract_mux4051.h"

Mux4051::Mux4051( byte a1Pin, byte a2Pin, byte a3Pin, byte chipEnablePin )
{
    this->a0Pin = a0Pin;
    this->a1Pin = a1Pin;
    this->a2Pin = a2Pin;

    pinMode( a0Pin, OUTPUT );
    pinMode( a1Pin, OUTPUT );
    pinMode( a2Pin, OUTPUT );
    
    if ( chipEnablePin >= 0 ) {
        this->chipEnablePin = chipEnablePin;
        pinMode( chipEnablePin, OUTPUT );
    } else {
        this->chipEnablePin = ABSTRACT_NOT_USED;
    }
}
    
void Mux4051::select( int address )
{
    digitalWrite( this->a0Pin, address & 1 );
    digitalWrite( this->a1Pin, address & 2 );
    digitalWrite( this->a2Pin, address & 4 );
    if ( this->chipEnablePin != ABSTRACT_NOT_USED ) {
        digitalWrite( this->chipEnablePin, LOW );
    }
}

void Mux4051::enable( boolean value )
{
    if ( this->chipEnablePin != ABSTRACT_NOT_USED ) {
        digitalWrite( this->chipEnablePin, value ? LOW : HIGH );
    }
}


Mux4051Pair::Mux4051Pair( int a0Pin, int a1Pin, int a2Pin, int a3Pin )
{
    this->wrapped = new Mux4051( a0Pin, a1Pin, a2Pin, a3Pin );
}

void Mux4051Pair::select( int address )
{
    this->wrapped->select( address );
    this->wrapped->enable( address & 8 );
}


MuxAnalogInput::MuxAnalogInput( Selector *mux, byte address, AnalogInput *input )
{
    this->mux = mux;
    this->address = address;
    this->input = input;
}

float MuxAnalogInput::get()
{
    this->mux->select( address );
    return this->input->get();
}


