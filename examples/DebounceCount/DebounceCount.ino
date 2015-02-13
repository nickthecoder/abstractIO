#include <abstractIO.h>

/*
Count the number of button pressed, and display the results by blinking an LED.

There are two buttons, one which is debounced, the other isn't.
The debounced button should work correctly all the time.
The other button may report too many button presses (because of the noisy switch).

I tried this with cheap momentary switches, and got bounced readings only about 1 in 20 times.

If you want to see large numbers of bounces, don't use a switch.
Instead connect a wire to the input pin, and then manually connect it directly to ground.
I get several bounces every time.
The debounced input still worked perfectly, even with such a noisey "switch" :-)
*/


Output* output1;

Button* buttonA;
Button* buttonB;

Button* goButton;

int count;

void setup()
{
    Serial.begin( 9600 );
    
    output1 = new SimpleOutput( 13 );
    
    buttonA = (new SimpleInput( 10 ))->button();
    buttonB = (new SimpleInput( 11 ))->debounced()->button();

    goButton = (new SimpleInput( 12 ))->debounced()->button();
    
    count = 0;
}

void loop()
{

    if ( buttonA->pressed() || buttonB->pressed() ) {
      count ++;
    }

    if ( goButton->pressed() ) {
        Serial.print( "There were " ); Serial.print( count ); Serial.println( " presses." );
        for( int i = 0; i < count; i ++ ) {
            output1->set( true );            
            delay( 100 );
            output1->set( false );  
            delay( 100 );
        }
        count = 0;
    }
}

