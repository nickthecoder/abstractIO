// See abstract_shift595SPI.h for why this has a weird .cpp.h suffix.

RemoteControl* remoteControl;

RemoteControl::RemoteControl( int pin )
{
  this->receiver = new IRrecv( pin );
  this->results = new decode_results;
  
  receiver->enableIRIn();
  remoteControl = this; // There's only one instance of receiver allowed.
}

void RemoteControl::loop()
{
    unsigned long tempValue = this->results->value;
    
    if (this->receiver->decode( this->results ) ) {
        if (this->results->value != REPEAT) {
            this->previousValue = tempValue;
        }
        this->receiver->resume();
    }
}

RemoteButton::RemoteButton( unsigned long value )
{
    this->value = value;
}

boolean RemoteButton::get()
{
    if ( remoteControl->results->value == REPEAT ) {
        return this->value == remoteControl->previousValue;
    } else {
        return this->value == remoteControl->results->value;
    }
}

boolean RemoteButton::pressed()
{
    return this->get();
}

boolean RemoteButton::released()
{
    return this->pressed();
}


