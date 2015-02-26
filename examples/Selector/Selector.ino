#include <abstractIO.h>
#include <abstract_shift595.h>
#include <abstract_shift164.h>
#include <abstract_lineDecoder138.h>

/*
Use a shift registor to act as a chip-selector.
i.e. give it an address (the chip number to enable) in the range 0..7, and the corresponding output of the shift register will be high.
All other outputs will be low.

In this example, we will drive two shift registers, a 595 and a 164, and also a line decoder 138.
You can wire up one or more chips. If you hang LEDs + resistors on the outputs, you can see the results.

The 164 only uses 2 pins, which makes it more applicable in most cases.
However, as it doesn't have a latch, intermediate outputs will be seen on its output's, but this is usually not a problem when used as a chip selector.
The 595 and the 138 should not show any intermediate outputs.
If you need 4 or fewer outputs, then the the 138 will only need 1 or 2 pins.

NOTE. The same code can be used to chain multiple 595s or 164s together, just chain the end of one register to the data-in of the next,
and change the values of "lines" as appropriate. Currently the code for the 138 doesn't support chaining.
*/

Shift595Selector *selector595;
//Shift164Selector *selector164;
//LineDecoder138 *selector138;

int lines = 6; // Number of output lines of the selector.

// If you connect all 8 lines to LEDs and then reduce the lines to 4, you will see that the first four lines
// are well behaved, and the last four will have junk in them. This is *not* a bug, its just a side effect of
// being efficient.

void setup()
{
    Serial.begin( 9600 );
    selector595 = new Shift595Selector( 12, 13, 11, lines ); // latch, clock, data, output line count
    //selector164 = new Shift164Selector( 2, 3, lines ); // clock, data, output line count
    //selector138 = new LineDecoder138( 4, 5, 6 ); // Address pins
}

void loop()
{ 
    Serial.println( "Up" );
    for ( int i = 0; i < lines - 1; i ++ ) {
        selector595->select( i );
        //selector164->select( i );
        //selector138->select( i );
        delay( 500 );
    }
    Serial.println( "Down" );
    for ( int i = lines - 1; i > 0; i -- ) {
        selector595->select( i );
        //selector164->select( i );
        //selector138->select( i );
        delay( 500 );
    }
}

