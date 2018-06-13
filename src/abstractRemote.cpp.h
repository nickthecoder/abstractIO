// See abstractRemote.h for why this has a weird .cpp.h suffix.

// REMOTE CONTROL

RemoteReceiver* RemoteReceiver;

RemoteReceiver::RemoteReceiver( byte pin )
{
    this->receiver = new IRrecv( pin );
    this->results = new decode_results;
  
    receiver->enableIRIn();
    RemoteReceiver = this; // There can only be one instance of remoteControl.
}

void RemoteReceiver::loop()
{
    if (this->receiver->decode( this->results ) ) {
        unsigned long tempValue = this->results->value;
        if (this->results->value == REPEAT) {
            this->value = this->previousValue;
        } else {
            this->previousValue = this->value;
            this->value = this->results->value;
        }
        this->receiver->resume();
    } else {
        this->value = 0l;
    }
}

// REMOTE BUTTON

RemoteButton::RemoteButton( unsigned long value )
{
    this->value = value;
}

boolean RemoteButton::get()
{
    return RemoteReceiver->value == this->value;
}

boolean RemoteButton::pressed()
{
    return RemoteReceiver->value == this->value;
}

boolean RemoteButton::released()
{
    return (RemoteReceiver->previousValue == this->value) && (RemoteReceiver->value != this->value );
}

// End
