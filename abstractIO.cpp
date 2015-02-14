/*
 * Copyright (c) 2015 Nick Robinson All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0 which accompanies this
 * distribution, and is available at http://www.gnu.org/licenses/gpl.html
*/

#include "abstractIO.h"

Input* Input::debounced()
{
    return new DebouncedInput( this );
}

Button* Input::button()
{
    return new Button( this );
}

SimpleInput::SimpleInput( int pin, byte trueReading, boolean enablePullup )
{
    this->pin = pin;
    this->trueReading = trueReading;
    
    pinMode( pin, INPUT );
    if ( enablePullup ) {
        digitalWrite( pin, HIGH ); // turn on pullup resistors
    }
}


boolean SimpleInput::get()
{
    return digitalRead( this->pin ) == this->trueReading;
}



DebouncedInput::DebouncedInput( Input* wrap, int debounceMillis )
  : wrapped(wrap)
{
    this->debounceMillis = debounceMillis;
    this->stableTime = millis();
    this->previousReading = wrap->get();
}

boolean DebouncedInput::get()
{
    boolean raw = this->wrapped->get(); // The current un-debounded value, which may be wrong due to noise.

    long now = millis();

    // If the switch changed, due to noise or pressing:
    if (raw != this->previousReading) {
        // reset the debouncing timer
        this->stableTime = now;
    }
    
    // Remember the current reading for the next time this function is called.
    this->previousReading = raw;
    
    if (now > this->stableTime + this->debounceMillis) {
        // whatever the reading is at, it's been there for longer
        // than the debounce period, so take it as the actual current state:
        this->stableState = raw;
        return raw;
    }
     
    return this->stableState;
}


CompoundInput::CompoundInput( Input **inputs, byte count )
{
    this->inputs = inputs;
    this->count = count;  
}

boolean CompoundInput::get()
{
    for ( int i = 0; i < this->count; i ++ ) {
        if (this->inputs[i]->get() ) {
            return true;
        }
    }
    return false;
}


Button::Button( Input* input )
  : input( input )
{
    this->previousState = this->input->get();
}

boolean Button::pressed()
{
    boolean state = this->input->get();
    if ( state && ! this->previousState ) {
        this->previousState = true;
        return true;
    }
    return false;
}

boolean Button::released()
{
    boolean state = this->input->get();
    if ( (!state) && this->previousState ) {
        this->previousState = false;
        return true;
    }
    return false;
}

boolean Button::get()
{
    return this->input->get();
}

SimpleOutput::SimpleOutput( int pin, boolean lowValue )
{
    this->pin = pin;
    this->lowValue = lowValue;
    pinMode( this->pin, OUTPUT );
}

void SimpleOutput::set( boolean value )
{
    digitalWrite( this->pin, value == lowValue ? LOW : HIGH );
}

BufferedOutput::BufferedOutput( byte *buffer, int index )
{
    this->mask = 1 << (index % 8);
    this->buffer = buffer + index / 8;
}

void BufferedOutput::set( boolean value )
{
    *this->buffer |= this->mask;
    if ( ! value ) {
        *this->buffer ^= this->mask;
    }
}


EasedAnalogInput* AnalogInput::ease( Ease* ease )
{
    return new EasedAnalogInput( this, ease );
}

ClippedAnalogInput* AnalogInput::clip( float minimum, float maximum )
{
    return new ClippedAnalogInput( this, minimum, maximum );
}

ScaledAnalogInput* AnalogInput::scale( float scale )
{
    return new ScaledAnalogInput( this, scale );
}

BinaryInput* AnalogInput::binary( float calibration, boolean reversed )
{
    return new BinaryInput( this, calibration, reversed );
}
    
SimpleAnalogInput::SimpleAnalogInput( int pin )
{
    pinMode( pin, INPUT );
    this->pin = pin;
}

float SimpleAnalogInput::get()
{
    float raw = analogRead( this->pin );
    return raw / 1023.0f; // NOTE. the range is 0..1 INCLUSIVE, therefore use 1023, not 1024.
}

ClippedAnalogInput::ClippedAnalogInput( AnalogInput* wrap, float minimum, float maximum )
  : wrapped( wrap )
{
    this->minimum = minimum;
    this->maximum = maximum;
}

float ClippedAnalogInput::get()
{
    float raw = this->wrapped->get();
    
    float range = this->maximum - this->minimum;
    
    float result = (raw - this->minimum) / range;

    if ( result < 0 ) {
        return 0;
    }
    if ( result > 1 ) {
        return 1;
    }
    
    return result;
}


ScaledAnalogInput::ScaledAnalogInput( AnalogInput* wrap, float scale )
{
    this->wrapped = wrap;
    this->scale = scale;
}

float ScaledAnalogInput::get()
{
    float raw = this->wrapped->get();
    
    return raw * this->scale;  
}

EasedAnalogInput::EasedAnalogInput( AnalogInput* wrap, Ease* ease )
  : wrapped( wrap ), ease( ease )
{
}

float EasedAnalogInput::get()
{
    return this->ease->ease( this->wrapped->get() );
}


BinaryInput::BinaryInput( AnalogInput *wrap, float calibration, boolean reversed )
{
    this->wrapped = wrap;
    this->calibration = calibration;
    this->reversed = reversed;
}

boolean BinaryInput::get()
{
    return (this->wrapped->get() < this->calibration) ^ this->reversed;
}

EasedPWMOutput* PWMOutput::ease( Ease *ease )
{
    return new EasedPWMOutput( this, ease );
}

SimplePWMOutput::SimplePWMOutput( int pin )
{
    this->pin = pin;
    pinMode( pin, OUTPUT );
}

void SimplePWMOutput::set( float value )
{
    int v = value * 255.0f;
    analogWrite( this->pin, v);
}


ScaledPWMOutput* PWMOutput::scale( float scale )
{
    return new ScaledPWMOutput( this, scale );
}



ScaledPWMOutput::ScaledPWMOutput( PWMOutput* wrap, float maximum )
  : wrapped( wrap ), scale( maximum )
{
}

void ScaledPWMOutput::set( float value )
{
    this->wrapped->set( value / this->scale );
};

EasedPWMOutput::EasedPWMOutput( PWMOutput* wrap, Ease* ease )
  : wrapped( wrap ), ease( ease )
{
}
  
void EasedPWMOutput::set( float value )
{
    this->wrapped->set( this->ease->ease( value ) );
}

float Linear::ease( float from  )
{
    return from;
}
Linear linear = Linear();

float Jump::ease( float from )
{
    return from < 0.5 ? 0 : 1;
}
Jump jump = Jump();



float EaseInQuad::ease( float from )
{
    return from * from;
}
EaseInQuad easeInQuad = EaseInQuad();


float EaseInCubic::ease( float from )
{
    return from * from * from;
}
EaseInCubic easeInCubic = EaseInCubic();

float EaseInQuart::ease( float from )
{
    return from * from * from * from;
}
EaseInQuart easeInQuart = EaseInQuart();



float EaseOutQuad::ease( float from )
{
    return 1 - easeInQuad.ease(1-from);
}
EaseOutQuad easeOutQuad = EaseOutQuad();

float EaseOutCubic::ease( float from )
{
    return 1 - easeInCubic.ease(1-from);
}
EaseOutCubic easeOutCubic = EaseOutCubic();


float EaseOutQuart::ease( float from )
{
    return 1 - easeInQuart.ease(1-from);
}
EaseOutQuart easeOutQuart = EaseOutQuart();


