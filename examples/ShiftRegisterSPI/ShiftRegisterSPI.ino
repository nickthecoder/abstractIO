/*
Use a shift registor to light 8 LEDs using SPI
For more information on SPI (including which pins to use), see : http://arduino.cc/en/Reference/SPI

Before looking at this example, please see ShiftRegister.
Does the same thing, but uses SPI hardware, which is quicker, but restricts you to using the
special SPI pins, which are on different pins, depending on the type of Arduino board you have.
*/

// The includes are different from the non-SPI version.
#include <SPI.h>
#include <abstract_shift595SPI.h>
#include <abstract_shift595SPI.cpp.h>
// Yes, this is weird, this does include C code, rather than just C headers.
// See abstract_shift595SPI.h for details of why I chose to do it this way.

// The name of the class is different from the non-SPI version.
Shift595SPI* shift;

Output* out0;
Output* out1;
Output* out2;
Output* out3;
Output* out4;
Output* out5;
Output* out6;
Output* out7;


int counter = 0;

void setup()
{
    Serial.begin( 9600 );

    // The construstor is different from the non-SPI version, as you can only choose the latch pin.
    // The other two pins MUST be the SPI clock and MOSI pins.
    shift = new Shift595SPI(2); // transfer using SPI hardware, faster, but must be on particular pins

    // The rest of the code is identical.

    out0 = new BufferedOutput( shift->buffer, 0 );
    out1 = new BufferedOutput( shift->buffer, 1 );
    out2 = new BufferedOutput( shift->buffer, 2 );
    out3 = new BufferedOutput( shift->buffer, 3 );
    out4 = new BufferedOutput( shift->buffer, 4 );
    out5 = new BufferedOutput( shift->buffer, 5 );
    out6 = new BufferedOutput( shift->buffer, 6 );
    out7 = new BufferedOutput( shift->buffer, 7 );
}

void loop()
{
    Serial.println( counter );

    // Display if the counter is divisible by 2,3,4...
    out0->set( counter % 2 ); 
    out1->set( counter % 3 ); 
    out2->set( counter % 4 ); 
    out3->set( counter % 5 ); 
    out4->set( counter % 6 ); 
    out5->set( counter % 7 ); 
    out6->set( counter % 8 ); 
    out7->set( counter % 9 ); 

    Serial.print( "Value " ); Serial.println( shift->buffer[0] );
    shift->update(); // Update the display
    delay( 1300 );
    counter ++;
}


