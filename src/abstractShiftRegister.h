#ifndef abstractShiftRegister_h
#define abstractShiftRegister_h

#include <Arduino.h>
#include <abstractIO.h>

class ShiftRegister;
class LatchedShiftRegister;
class BufferedShiftRegister;
class ShiftRegisterSelector;
class ComboSelector;

/*
 * An unlatched shift register, such as a 74xx164.
 * Note, I prefer to use a LatchedShiftRegister, such as a 74xx595 in more scenarios.
 */
class ShiftRegister
{
  protected :
    byte clockPin;
    byte dataPin;
    boolean order;
    
  public:
    ShiftRegister( byte dataPin, byte clockPin, boolean order=MSBFIRST /* or LSBFIRST*/ );
    
    /*
     * Shifts out a single byte.
     */
    void output( byte value );

    /*
     * Shifts out two bytes
     */
    void output( byte first, byte second );
    
    /*
     * Shifts out an array of bytes.
     */
    void output( byte byteCount, byte *values );
    
    virtual void latchOutput(); /* Does nothing. the LatchedShiftRegister overrides this */
    
    /*
     * Shift in n bits of data, with the given value. Note, latchOutput() is not called.
     */
    void shift ( boolean value, byte n = 1 );

    BufferedShiftRegister* buffer( byte byteCount );
};

/*
 * A latched shift register, such as a 74xx595.
 */
class LatchedShiftRegister : public ShiftRegister
{
  protected :
    byte latchPin;
        
  public :
    LatchedShiftRegister( byte dataPin, byte clockPin, byte latchPin, boolean order=MSBFIRST /* or LSBFIRST*/ );
        
    virtual void latchOutput();
};

/*
 * Keeps the state of the shift register in a buffer.
 * This is useful in conjunction with BufferedOutput.
 */
class BufferedShiftRegister
{
  public :
    ShiftRegister *shiftRegister;

  public :      
    byte byteCount;
    byte *buffer;

    BufferedShiftRegister( ShiftRegister *shiftRegister, byte byteCount );
    
    /*
     * Sets an individual bit of the buffer.
     * When you have finished updating the required bits, call update() to ouput the changes to the shift register.
     */
    void set( byte index, boolean value );
    
    /*
     * Copies the value in the buffer into the shift register.
     */
    void update();
    
    /*
     * Create an array of Output objects. The size of the array is 8 * byteCount (passed to the constructor).
     */
    BufferedOutput** createOutputs();
};

/*
 * Uses a ShiftRegister to act as a Selector.
 * i.e. calling select() will cause just one of the shift registers outputs to be LOW, and the others to be HIGH.
 * You can reverse the logic (so that one ouput is HIGH, by setting activeHighLow in the constructor to HIGH).
 * 
 * If you only need upto 8 chip-select lines, then a 74xx138 (using the LineDecoder class) will be quicker, and will
 * use the same number of address lines.
 * 
 * However, shift registers can be easily chained, so you can drive an unlimited number of chip-select lines using just 3 pins
 * (data, clock and latch).
 */ 
class ShiftRegisterSelector : public Selector
{
  protected :
    ShiftRegister *shiftRegister;
    byte addresses;
    boolean activeHighLow;
    byte previousAddress;
  
  public :
    ShiftRegisterSelector( ShiftRegister *shiftRegister, byte addresses, boolean activeHighLow = LOW );
    virtual void select( byte address );
};

/*
 * Uses a ShiftRegister as a Selector.
 * Three bits of the output are used to drive a three bit address, and the remaining 5 bits are used as
 * chip selector lines. This allows a single 8 bit shift register to select upto 40 channels (8x5).
 * 
 * I use this as the Selector in combination with upto 5 4051 multiplexers, so that upto 40 channels
 * can be multiplexed into one input, using just 3 control pins.
 * 
 * Note, this implementation does NOT allow the shift registers to be chained, so the address range is
 * limited to 0..39.
 * 
 * If you use the default order of MSBFIRST in the constructor to the ShiftRegister, then the address
 * will appear on Q0, Q1 and Q2 of the shift register's outputs, and the chip-select lines on
 * the remaining outputs.
 */
class ComboSelector : public Selector
{
  protected :
    ShiftRegister *shiftRegister;
    boolean activeHighLow;

  public :
    ComboSelector( ShiftRegister *shiftRegister, boolean activeHighLow = LOW );
    
    virtual void select( byte address /* 0..39 */ );
};

#endif
