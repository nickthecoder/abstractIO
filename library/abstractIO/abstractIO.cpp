/*
 * Copyright (c) 2015 Nick Robinson All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0 which accompanies this
 * distribution, and is available at http://www.gnu.org/licenses/gpl.html
*/

#include "abstractIO.h"

AbstractSerial::AbstractSerial( int baud ) {
    Serial.begin( baud );
    
};

// INPUT

Input* Input::debounced()
{
    return new DebouncedInput( this );
}

// INPUT BUTTON

InputButton* Input::button()
{
    return new InputButton( this );
}

// SIMPLE INPUT

SimpleInput::SimpleInput( int pin, boolean trueReading, boolean enablePullup )
{
    this->pin = pin;
    this->trueReading = trueReading;
    
    pinMode( pin, enablePullup ? INPUT_PULLUP : INPUT );
}


boolean SimpleInput::get()
{
    return digitalRead( this->pin ) == this->trueReading;
}


// MUX

Mux::Mux( Selector *selector, byte inputPin )
{
    this->selector = selector;
    this->input = new SimpleInput(inputPin);
}

Mux::Mux( Selector *selector, Input* input )
{
    this->selector = selector;
    this->input = input;
}

Input* Mux::createInput( byte address )
{
    return new MuxInput( this, address );
}

boolean Mux::get( byte address )
{
    this->selector->select( address );
    return this->input->get();
}

// MUX INPUT

MuxInput::MuxInput( Mux *mux, byte address )
{
    this->mux = mux;
    this->address = address;
}

boolean MuxInput::get()
{
    return this->mux->get( address );
}

// DEBOUNCED INPUT

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

DebouncedInput::DebouncedInput( Input* wrap, int debounceMillis )
  : wrapped(wrap)
{
    this->debounceMillis = debounceMillis;
    this->stableTime = millis();
    this->previousReading = wrap->get();
}

// DELAY PERIOD
unsigned long delayPeriodPrev = 0;

void delayPeriod( int period ) {

    unsigned long now = millis();
    unsigned long diff = now - delayPeriodPrev;
    delayPeriodPrev = now;
    if ( diff < period ) {
        delay( diff );
    } else if ( diff > 1000000000L ) {
        // Hopefully, this tests for when millis() has rolled over, but as it takes 50 days,
        // I haven't actually tested it!!!
        // After a rollover, wait the maximum time. Not perfect, but good enough I think.
        delay( period );
    }
}

// RUN PERIODICALLY
RunPeriodically::RunPeriodically( unsigned long millis, void (*callback)(void) ) {
    this->period = millis;
    this->prevMillis = 0L;
    this->callback = callback;
}

void RunPeriodically::run() {
    unsigned long diff = millis() - this->prevMillis;
    if ( diff > period ) {
        this->prevMillis = millis();
        this->callback();
    }
}
                                  
// BINARY INPUT

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

// SELECTOR

Mux* Selector::createMux( byte inputPin )
{
  return new Mux( this, inputPin );
}

Mux* Selector::createMux( Input* input )
{
  return new Mux( this, input );
}

AnalogMux* Selector::createAnalogMux( byte inputPin )
{
  return new AnalogMux( this, new SimpleAnalogInput( inputPin ) );
}

AnalogMux* Selector::createAnalogMux( AnalogInput *analogInput )
{
  return new AnalogMux( this, analogInput );
}

// ADDRESS SELECTOR

AddressSelector::AddressSelector( byte pinCount, byte *addressPins )
{
    this->pinCount = pinCount;
    this->addressPins = addressPins;
    
    for ( byte i = 0; i < pinCount; i ++ ) {
        pinMode( addressPins[i], OUTPUT );
    }
    
    select( 0 );
}

AddressSelector::AddressSelector( byte a0, byte a1, byte a2 )
{
    this->pinCount = 3;
    this->addressPins = (byte*) malloc( 3 );
    this->addressPins[0] = a0;
    this->addressPins[1] = a1;
    this->addressPins[2] = a2;
    
    pinMode( a0, OUTPUT );
    pinMode( a1, OUTPUT );
    pinMode( a2, OUTPUT );
    
    select( 0 );
}

void AddressSelector::select( byte address )
{
    byte digit = 1;
    for ( byte i = 0; i < pinCount; i ++ ) {
        digitalWrite( this->addressPins[i], address & digit );
        digit = digit << 1;
    }
}

// BOOLEAN SELECTOR

BooleanSelector::BooleanSelector( byte selectPin )
{
    this->selectPin = selectPin;
    pinMode(selectPin, OUTPUT);
}

void BooleanSelector::select(byte address)
{
    digitalWrite( selectPin, address );
}

// INPUT BUTTON

InputButton::InputButton( Input* input )
  : input( input )
{
    this->pressedDetected = false;
    this->releasedDetected = false;
}

boolean InputButton::get()
{
    return this->input->get();
}

boolean InputButton::pressed()
{
    boolean state = this->input->get();
    if ( state && ! this->pressedDetected ) {
        this->pressedDetected = true;
        this->releasedDetected = false;
        return true;
    } else {
        this->pressedDetected = state;
    }
    return false;
}

boolean InputButton::released()
{
    boolean state = this->input->get();
    if ( (!state) && ! this->releasedDetected ) {
        this->releasedDetected = true;
        this->pressedDetected = false;
        return true;
    } else {
        this->releasedDetected = ! state;
    }
    return false;
}

// COMPOUND BUTTON

CompoundButton::CompoundButton( Button *buttonA, Button *buttonB )
{
    this->buttonA = buttonA;
    this->buttonB = buttonB;
}

boolean CompoundButton::get()
{
    return this->buttonA->get() || this->buttonB->get();
}

boolean CompoundButton::pressed()
{
    return this->buttonA->pressed() || this->buttonB->pressed();
}

boolean CompoundButton::released()
{
    return this->buttonA->released() || this->buttonB->released();
}

// SIMPLE OUTUT

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

// BUFFERED OUTPUT

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

// ANALOG INPUT

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

// SIMPLE ANALOG INPUT

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

// ANALOG MUX

AnalogMux::AnalogMux( Selector *selector, AnalogInput *input )
{
    this->selector = selector;
    this->input = input;
}

float AnalogMux::get( byte address )
{
    this->selector->select( address );
    return this->input->get();
}

AnalogInput* AnalogMux::createInput( byte address )
{
    return new AnalogMuxInput( this, address );
}

// MUX ANALOG INPUT

AnalogMuxInput::AnalogMuxInput( AnalogMux *mux, byte address )
{
    this->mux = mux;
    this->address = address;
}

float AnalogMuxInput::get()
{
    return this->mux->get( this->address );
}

// CLIPPED ANALOG INPUT

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

// SCALED ANALOG INPUT

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

// EASED ANALOG INPUT

EasedAnalogInput::EasedAnalogInput( AnalogInput* wrap, Ease* ease )
  : wrapped( wrap ), ease( ease )
{
}

float EasedAnalogInput::get()
{
    return this->ease->ease( this->wrapped->get() );
}

// PWM OUTPUT

EasedPWMOutput* PWMOutput::ease( Ease *ease )
{
    return new EasedPWMOutput( this, ease );
}


ScaledPWMOutput* PWMOutput::scale( float scale )
{
    return new ScaledPWMOutput( this, scale );
}

// SIMPLE PWM OUTPUT

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

// SCALED PWM OUTPUT

ScaledPWMOutput::ScaledPWMOutput( PWMOutput* wrap, float maximum )
  : wrapped( wrap ), scale( maximum )
{
}

void ScaledPWMOutput::set( float value )
{
    this->wrapped->set( value / this->scale );
};

// EASED PWM OUTPUT

EasedPWMOutput::EasedPWMOutput( PWMOutput* wrap, Ease* ease )
  : wrapped( wrap ), ease( ease )
{
}
  
void EasedPWMOutput::set( float value )
{
    this->wrapped->set( this->ease->ease( value ) );
}

// EASE (various)

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

// END
