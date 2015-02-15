#include <SPI.h>

#include <abstractIO.h>
#include <abstract_shift595.h>

/*
Use a shift registor to light 8 LEDs

The use of BufferedOutput may seen silly, because it would be easier to just write to the buffer
directly rather than use Output::set().
However, imagine you start writing a project, and have plenty of pins for your outputs, so you use
SimpleOutput (and no shift register). You project grows, and you run out of pins.
Instead of rewriting your project, or buying an Arduino Mega, wire up the shift register,
change the setup code, add the extra 'shift.update();' statement in the loop() function.
Your application now has access to more outputs, without any major changes.

NOTE, If you've used delay(), for timings, then this won't work without sprinkling shift->update() after
each delay. Using delay is best avoided though!
*/

Shift595* shift;

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
    shift = (new Shift595())->pins( 2 ); // transfer using SPI hardware, faster, but must be on particular pins
    // shift = (new Shift595())->pins( 2, 13, 11 ); // shiftOut using Software, on arbirary pins

    
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


