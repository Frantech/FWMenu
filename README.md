# FWMenu
Arduino driven interactive menu interface for a TM1638 7 segment LED display module.


This library allows a menu to be defined with variables and values set at compile time. 
At run time the values of the variables can be modified by pressing the buttons on the hardware. 
All of the values used can be read by the Arduino software immediately in real time.

The code is provided as a C++ library with two examples of it's use. 
It has been written with the intention of a making it easy to set up. 
Further description can be found in the code files FWMenu.h and FWMenu.cpp.

This will be useful for any application where an electronic device needs a bright LED display 
to control parameters used by the device. An example could be a CNC tool that cuts by 
measurements of Length, Width and Height. See the examples given, but the possibilities are
only limited by your imagination.
