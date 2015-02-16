/*
This requires the IRremote library, which is not included as standard in the Arduino IDE.
You can get a copy from here :
https://github.com/shirriff/Arduino-IRremote
(Read the "readme" at the bottom of the page for how to install the library).

This makes use of the IRremote library, and because the Arduino IDE is trying cleverer than
it is, that causes a big problem! To get around this problem, I use a horrible bodge...
To include this code into your sketch you need to include the following :

#include <IRremote.h>
#include <abstract_remote.h>
#include <abstract_remote.cpp.h>

If you have split your code into different files, only include <abstract_remote.cpp.h> once.

What this does, is compile this code into YOUR sketch, rather than into the abstractIO library.
This makes it nice and simple for most people, and just a bit weird for people who want to use
RemoteControl and RemoteButton.

This is a horrible bodge, and I know it, so if you know of a better way to do it, let me know!
(I don't want to make everybody #include <IRremote.h> when they aren't using it).
*/
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

