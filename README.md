Do you want to use some object oriented goodness to hide some of the nitty gritty stuff
from your application code?

AbstractIO is designed to keep your application clean and simple. Your setup code is the
only place with the details of how your project is wired.

Here's some examples

* Your application code shouldn't need to know if a button press is a LOW or HIGH signal.
* Checking the state of a switch should be the same whether you have just one switch or hundreds (multiplexing using a 74HC595).
* Turning on an LED should be the same whether you have just one, or dozens (using shift registers).
* Fading an LED should be no different when using external PWMs such as WS2803D or TLC5940.
* Your application code shouldn't care if an input is a simple switch or a signal from an infra-red remote control.
* Some inputs need debouncing? Hide that from your application code!
* Are you using a potentiometer or an rotary encoder as a numeric input? Make them behave identically.
* Got a light dependent resistor on an analog pin? Calibrate the readings, then make it appear like a regular on/off input.


To Do
=====

Create test scripts.
Create example code.
Tidy up, place it into my Library folder
Store on git hub


Bugs/Bad Style
--------------

Use [] syntax for CompoundInput
Use references rather than pointers

Input
-----

isPressed for Input - only true once when a button has been pressed, until it is released
Maybe also input handlers, so that functions are automagically called.
Implement a multiplexer

AnalogInput
-----------

Implement a multiplexed input

Output
------

Implement a shift register

PWMOutput
---------

Implement an external PWM output




