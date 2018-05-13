#include <abstractIO.h>
#include <abstractShiftRegister.h>

/*
Use a shift registor to act as a chip-selector.
i.e. give it an address (the chip number to enable) in the range 0..7, and the corresponding output of the shift register will be high.
All other outputs will be low.
*/

// Number of output lines of the selector.
// Use 2 when testing the BooleanSelector, 8 for for the ShiftRegisterSelector, 40 for the ComboSelector
// 8 for a single 74xx138 line decoder. 16 for two of them.
const int lines = 16;

// Choose one of the following :

// The simplest chip selector using just one pin, with an external inverter to select one of just 2 chip select lines.
//Selector *selector = new BooleanSelector( 10 );

// 74xx595 shift register to operate 8 chip select lines. Note, can be chained to give as many chip select lines as you need.
//Selector *selector = new ShiftRegisterSelector( new LatchedShiftRegister(2,4,3 /*data, clock, latch */), lines );

// An unlatched shift register. You could also uses this with a 74xx595, and tie the latch HIGH.
//Selector *selector = new ShiftRegisterSelector( new ShiftRegister( 2, 4 /* data, clock */ ), lines );

// A 74xx595 where the lowest 3 bits are used as address lines (8 addresses), and the top 5 bits are used as chip select lines (active LOW).
// I use this as the selector for a set of 4051 multiplexers, giving a maximum of 40 sources.
// Note, this does NOT support chaining of the 595.
//Selector *selector = new ComboSelector( new LatchedShiftRegister(2,4,3 /*data, clock, latch */), lines );

// Use a Line Decoder chip, such as the 74xx138. for upto 8 chip select lines.
//Selector *selector = new AddressSelector( 4, 5, 6 /* Address pins A,B,C*/ );

// Use a pair of 74xx138 Line Decoder chips for upto 16 lines.
// The first three pins are used as the "Select Inputs" A,B,C.
// The last pin (pin 4) is wired to the one of the 74xx138's G1, and the other's G2A. (Called "Enable Inputs").
// PS. I haven't tested this as I don't have any 74xx138 at hand. Oops sorry. I think it should work though!
// You can do a similar trick using the G2B with a 5th address bit, but this only delivers 24 chips select lines (using 3 74xx138s).
// You would need an additional external inverter to deliver the full 32 chip select lines (using 4 74xx138s).
byte addressPins[4] = {2, 3, 4, 5};
Selector *selector = new AddressSelector( 4 /* array size */, addressPins );

void setup()
{
    Serial.begin( 9600 );
}

void loop()
{ 
    Serial.println( "Up" );
    for ( int i = 0; i < lines - 1; i ++ ) {
        selector->select( i );
        delay( 500 );
    }
    Serial.println( "Down" );
    for ( int i = lines - 1; i > 0; i -- ) {
        selector->select( i );
        delay( 500 );
    }
}

