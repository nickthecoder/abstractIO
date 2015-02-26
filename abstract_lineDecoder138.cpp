
#include "abstract_lineDecoder138.h"

LineDecoder138::LineDecoder138( byte a0, byte a1, byte a2 )
{
    pinMode( a0, OUTPUT );
    if ( a1 != ABSTRACT_NOT_USED ) {
        pinMode( a1, OUTPUT );
    }
    if ( a2 != ABSTRACT_NOT_USED ) {
        pinMode( a2, OUTPUT );
    }
    
    this->a0 = a0;
    this->a1 = a1;
    this->a2 = a2;
    
    select( 0 );
}

void LineDecoder138::select( int address )
{
    digitalWrite( this->a0, address & 1 );

    if ( this->a1 != ABSTRACT_NOT_USED ) {
        digitalWrite( this->a1, address & 2 );
    }
    
    if ( this->a2 != ABSTRACT_NOT_USED ) {
        digitalWrite( this->a2, address & 4 );
    }
}


