/*
AbstractIR *pInstance;

AbstractIR::AbstractIR( int pin )
{
    pInstance = this;
    pReceiver = new IRrecv( pin );
    pReceiver->enableIRIn();
}

AbstractIR::loop()
{
    boolean result = receiver.decode(&this->results);
    if (result) {
        this->value = this->results.value;
        this->receiver.resume();
    } else {
        this->value = 0;
    }
}


IRInput::IRInput( long value )
{
    this->value = value;
}

boolean IRInput::get()
{
    return pInstance->value == this->value;
}
*/


