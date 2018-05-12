/*
Use shift registors to light any number of LEDs in sequence.
*/

#include <abstractIO.h>
#include <abstractShiftRegister.h>

// Create the shift register.
ShiftRegister* shift = new LatchedShiftRegister( 2 /*data pin */ ,4 /*clock pin*/, 3 /*latch pin*/);

void setup()
{
    Serial.begin( 9600 );
}

void loop()
{
    // Use one shift register to count up to 256 in binary
    /*
    for ( int counter = 0; counter < 256; counter ++ ) {
        Serial.println( counter );
        shift->output( counter );
        delay( 40 );
    }
    */  
    
    // Use 2 shift registers to count up to 65536 in binary.
    byte bytes[2]; // To send multiple bytes to the shift register, we need to pass an array of bytes.
    for ( int counter = 0; counter < 65536; counter ++ ) {
        Serial.println( counter );

        // Note, we would swap the parameters if we were sending the data least significant bit first
        // i.e. if LSBFIRST were passed as the "order" parameter to the shift register's constructor.        
        shift->output( counter >> 8, counter & 0xff ); // Output the 2 bytes of data
        delay( 40 );
    }
}


