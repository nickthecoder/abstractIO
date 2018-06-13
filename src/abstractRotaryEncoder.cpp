#include "abstractRotaryEncoder.h";

// ROTARY ENCODER

REAnalogInput* RotaryEncoder::createAnalogInput( int steps )
{
    return new REAnalogInput( this, steps );
}

// SIMPLE ROTARY ENCODER

SimpleRotaryEncoder::SimpleRotaryEncoder( byte pinA, byte pinB, byte beatsPerDetent )
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->val = 0;
    this->beatsPerDetent = beatsPerDetent;
    
    pinMode( pinA, INPUT_PULLUP );
    pinMode( pinB, INPUT_PULLUP );
    
    this->aState = digitalRead( pinA );
}

int SimpleRotaryEncoder::get()
{
    boolean newState = digitalRead( pinA );
    
    if (this->aState != newState) {
        if ( digitalRead( pinB ) != newState ) {
            this->val --;
        } else {
            this->val ++;
        }
        this->aState = newState;
    }
    
    return this->val / beatsPerDetent;
}

void SimpleRotaryEncoder::set( int value )
{
    this->val = value * this->beatsPerDetent;
}

// ROTARY ENCODER ANALOG INPUT

REAnalogInput::REAnalogInput( RotaryEncoder* re, int steps )
{
    this->re = re;
    this->steps = steps;
}

float REAnalogInput::get()
{
    int value = this->re->get();
    if ( value < 0 ) {
        this->re->set(0);
        return 0;
    } else if ( value > steps ) {
        this->re->set(steps);
        return 1;
    } else {
        return value / (float) this->steps;
    }
}

void REAnalogInput::set( float value )
{
    this->re->set( steps * value );
}
