/*
This requires the IRremote library, which is not included as standard in the Arduino IDE.
You can get a copy from here :
https://github.com/shirriff/Arduino-IRremote
(Read the "readme" at the bottom of the page for how to install the library).

This makes use of the IRremote library, and because the Arduino IDE is trying to be cleverer than
it is, that causes a big problem! To get around this problem, I use a horrible bodge...
To include this code into your sketch you need to include the following :

#include <IRremote.h>
#include <abstractRemote.h>
#include <abstractRemote.cpp.h>

If you have split your code into different files, only include <remote.cpp.h> once.

What this does, is compile this code into YOUR sketch, rather than into the abstractIO library.
This makes it nice and simple for those not using these classes, and just a bit weird for people who use them.

This is a horrible bodge, and I know it, so if you know of a better way to do it, let me know!
(I don't want to make everybody #include <IRremote.h> when they aren't using it).
Hopefully, at some point, the IDE will stop trying to be clever, or actually BE clever and this bodge won't be needed!
*/
#ifndef abstractRemote_h
#define abstractRemote_h

#include <IRremote.h>
#include <abstractIO.h>

class RemoteReceiver;
class RemoteButton;


class RemoteReceiver
{
  friend RemoteButton;
  
  private :
    IRrecv *receiver;
    decode_results * results;

    unsigned long previousValue;
    unsigned long value;

  public :
    RemoteReceiver( byte receiverPin );
    
    void loop(); // Call this from your sketch's loop() function (must be called frequently!)
    
    // Creates a Button object for a given key on the remote control.
    // value : The code for the key. You can find the codes, by running the example code in IRremote's library.
    RemoteButton* button( long value );
    
    friend RemoteButton;
};

/*
 * Unlike a wired button, we can't know for sure when an IR button had been released,
 * so rather than try to guess, pressed() returns the same as get() and released() always returns false.
 * If you hold down a remote's button, you will end up with multiple pressed signals only if the remote control supports
 * repeats.
 */
class RemoteButton : public Button
{
  protected :
    unsigned long value;
    
  public :
    RemoteButton( unsigned long value );
    
    virtual boolean pressed(); // Returns the same as get(), as we cannot detect when a button is held down.
    
    virtual boolean released(); // We cannot really know when a remote control button has been released, so don't rely on this method!
    // It is only here to conform with the Button interface.
    
    virtual boolean get();

};

#endif

