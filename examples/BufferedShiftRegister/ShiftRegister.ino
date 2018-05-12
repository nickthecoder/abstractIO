/*
Use shift registors to light any number of LEDs in sequence.
*/

#include <abstractIO.h>
#include <abstractShiftRegister.h>

// The number of 595 shift registers chained together.
const int byteCount = 1;

// Create the shift register.
BufferedShiftRegister *shift = (new LatchedShiftRegister(2 /*data*/ ,4 /*clock*/, 3 /*latch*/))->buffer( byteCount );

// Create an array of pointers to BufferedOutput objects.
// We can then set the outputs as we wish using :
//     outputs[n]->set(value);
// and then call :
//     shift->update()
// to see the results.
BufferedOutput** outputs = shift->createOutputs();

void setup()
{
    Serial.begin( 9600 );
}

void loop()
{
    for ( int counter = 0; counter < 1000; counter ++ ) {

        // Set each of the outputs.
        for ( int i = 0; i < byteCount * 8; i ++ ) {
            // outputs[ i ]->set( counter & ( 1 << i ) ); // Binary counter
            //outputs[ i ]->set( (counter % (8*byteCount)) == i ); // Simple moving LED
            outputs[ i ]->set( (counter % (i+2)) == 0 ); // Is counter divisible by 2, 3, 4, 5, 6, 7...
        }
        Serial.println();
        shift->update(); // Update the display
        delay( 400 );
    }
}


