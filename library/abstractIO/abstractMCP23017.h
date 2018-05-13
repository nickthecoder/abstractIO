/*
 * Interfaces with the MCP23017 using I2C interface.
 * This only gives access to basic IO, it does NOT support the interrupt pins.
 * 
 * http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf
 *
 * NOTE. This code uses Wire.h to communicate with the MCP23017, and Wire.h doesn't work before setup() has been called.
 * Therefore do NOT attempt to use these class before setup() has been called.
 * This include createInput() and createOutput(), so you cannot create input and output objects in the global
 * variable definitions. You have to create them in setup().
*/

#ifndef abstractMCP23017_h
#define abstractMCP23017_h

#include <Arduino.h>
#include <abstractIO.h>

/*
 * Base class for MCP23017 and BufferedMCP23017.
 */
class AbstractMCP23017 {

  protected :
    byte i2cAddress;
    
  public :
    AbstractMCP23017( byte address );
      
    void pinMode( byte pinNumber /* 0..15 */, byte mode /*INPUT, OUTPUT or INPUT_PULLUP */ );
    
    // If value is true, then the input logic is reversed (useful for buttons with pullup resistors).
    void inputPolarity( byte pinNumber /* 0..15 */, boolean value );

    virtual boolean digitalRead( byte pinNumber /* 0..15 */ ) = 0;
    virtual void digitalWrite( byte pinNumber /* 0..15 */, boolean value ) = 0; 
    
    // Reads all 16 pins. Bank A is in the lowest 8 bits.
    unsigned int readBoth();
    // Reads a single bank of inputs, returning an 8 bit value.
    byte readBank( boolean bankA = true );
    
    // write to both banks. A is in the low order 8 bits of data.
    void writeBoth( unsigned int data );
    // Write to a single bank
    void writeBank( boolean bankA, byte data );
    
    Input* createInput( byte pinNUmber, boolean trueReading = LOW /* or HIGH */, boolean enablePullup = false );
    Output* createOutput( byte pinNUmber );

  protected :
    byte readRegister( byte registerID ); // Reads a single register
    unsigned int readRegister2( byte registerID ); // Reads two consecutive registers

    void writeRegister( byte registerID, byte value ); // Writes a single register
    void writeRegister2( byte registerID, unsigned int value ); // Writes two consecutive registers.

    void setRegisterBit( byte registerID, byte bit, boolean value ); // Reads, adjusts one bit, then writes.

};

/*
 * The "normal" (unbuffered) implementation to interface with the MCP23017 IO Expander via I2C.
 * Methods digitalRead() and digitalWrite() are slow compared to the BufferedMCP23017, but this
 * class is easier to use.
 */
class MCP23017 : public AbstractMCP23017 {
    
  public :
    MCP23017( byte address = 0 );
        
    // Read the state of a single pin.
    virtual boolean digitalRead( byte pinNumber /* 0..15 */ );
    
    // Write the output of a single pin.
    // Note, this performs a read, adjusts one bit, then writes.
    virtual void digitalWrite( byte pinNumber /* 0..15 */, boolean value ); 
    
};

/*
 * Provides buffered access to the MCP23017 for faster digitalRead and digitalWrite.
 * 
 * To read values call read(), to update the buffer, then use digitalRead to read the pin values.
 * To write values, call digitalWrite, and then call flush() to update all of the data.
 * 
 * A typical application can call read() once at the start of the main loop() method, and flush() once at the end of the loop() method.
 * digitalRead() and digitalWrite() can then be called wherever needed.
 */
class BufferedMCP23017 : public AbstractMCP23017 {
    
  public :
    BufferedMCP23017( byte address = 0 );
        
    // Read the state of a single pin.
    boolean digitalRead( byte pinNumber /* 0..15 */ );
    
    // Write the output of a single pin.
    void digitalWrite( byte pinNumber /* 0..15 */, boolean value ); 

    void read(); // Causes the next call to digitalRead to update the readBuffer
    void flush(); // Causes the outputBuffer to be written to the chip (if it differs from oldOutputBuffer).
    
  protected :
    unsigned int inputBuffer;
    unsigned int outputBuffer;
    unsigned int oldOutputBuffer; // When outputBuffer != oldOutputBuffer, then a write is performed in flush().
    boolean readRequired; // Set within read(), and reset within digitalRead().
};

class MCP23017Input : public Input {
  public :
    MCP23017Input( AbstractMCP23017* mcp23017, byte pinNumber, boolean trueReading = LOW/* or HIGH */, boolean enablePullup = false );
  
    virtual boolean get();
    
  protected :
      AbstractMCP23017* mcp23017;
      byte pinNumber;
};

class MCP23017Output : public Output {
  public :
    MCP23017Output( AbstractMCP23017* mcp23017, byte pinNumber );
  
    virtual void set( boolean value );
    
  protected :
      AbstractMCP23017* mcp23017;
      byte pinNumber;
};

#endif
