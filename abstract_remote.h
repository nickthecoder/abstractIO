#ifndef abstract_remote_h
#define abstract_remote_h

#include <IRremote.h>
#include "abstractIO.h"

class RemoteControl;
class RemoteButton;


class RemoteControl
{
  public :
    unsigned long value;
    boolean repeated;
    
  private :
    IRrecv *receiver;
    decode_results * results;

    unsigned long previousValue;

  public :
    RemoteControl( int receiverPin );
    
    void loop(); // Need to call this from your sketch's loop() function. 
    
    RemoteButton* button( long value );
    
    friend RemoteButton;
};

/*
Unlike a wired button, we can't know for sure when an IR button had been released,
so rather than try to guess, each IR signal is assumed to be a press and instant release.
i.e. pressed(), released() and get() all give the same results.
If you hold down a remote's button, you will end up with multiple pressed/released signals.
*/
class RemoteButton : public Button
{
  public :
    unsigned long value;
    
  public :
    RemoteButton( unsigned long value );
    
    virtual boolean pressed();
    virtual boolean released();    
    virtual boolean get();

};


#endif

