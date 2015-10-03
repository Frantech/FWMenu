//
//    FILE: FWMenu.h
//  AUTHOR: Frantech
// VERSION: 0.1.00
// PURPOSE: Interactive menu for TM1638 7 segment display
//     URL: https://github.com/Frantech/FWMenu
//REQUIRES: TM1638 library
//
/*
    FWMenu – Interactive Menu Interface for a TM1638 LED Module.
                  Copyright © 2015 Frank Williams

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

The author can be contacted at frank.w2@paradise.net.nz or by writing to:
    Frank Williams
    89 Miro St
    Trentham
    Upper Hutt
    New Zealand 5018   

            **** IMPORTANT Prerequisite Install ****	
This code builds on the work done by Ricardo Batista and requires the 
    preinstall of his TM1638 software which is available from 
    https://code.google.com/p/tm1638-library/     

	*/

#include "Arduino.h"
#include <TM1638.h>

/*
  FWMenu - Menu Program for TM1638 LED module
  ===========================================
 
 FWMenu.h - Library for creating an interactive menu for use with a TM1638 module.
 Created by Frank Williams,  May 17, 2015.
 
 For instructions see FWMenu.cpp
 */

// Inclusion guard. 
#ifndef fwmenu_h
#define fwmenu_h

/* Define which button is B1 etc. If you want to use different buttons than what is given, 
 this is where to do it. (Swap the hex values) */
#define B1 0x01
#define B2 0x02
#define B3 0x04
#define B4 0x08
#define B5 0x10
#define B6 0x20
#define B7 0x40
#define B8 0x80

#define maxMenuSize 16

// Class definition - FWMenu is a derived class, extends TM1638.
class FWMenu: 
public TM1638{

public:
  // Constructor with initialisation list (for the base class). 
  FWMenu(byte dataPin, byte clockPin, byte strobePin, boolean activateDisplay, byte intensity);

  // Accessor functions.
  void setButtonz(byte b){
    buttonz = b;
  }
  byte getButtonz(){
    return buttonz;
  }        

  // NB - no setp() method because p is set only by addToMenu().
  int getp(){
    return p;
  }

  // One off actions to get set up. Call this from within setup() method. Default speed is 500 milliseconds.
  void setupMenu(int pace=500);

  // Set up menu storage. Each call adds one option value pair to the menu. 
  // Call this from setup(). Once per menu item.
  void addToMenu(char opt[],float val,char attr,float delta);

  // This is the main process. It needs to be called from inside the loop() method.
  void useMenu();

  // Get a menu value.
  float getMenuValueForOption(char opt[]);

  // Reset all menu values to defaults.
  void resetMenu();

  // Function buttonPressed() - returns 1 or 0 to indicate if a button is pressed. 
  boolean aButtonPressed(byte btn);

  // Button number masks. 
  byte button1;
  byte button2;
  byte button3;
  byte button4;
  byte button5;
  byte button6;
  byte button7;
  byte button8;   

private:

  // Variable used for sensing buttons.
  byte buttonz;

  // Variables for menu selections code.
  int _pace;   	      // Speed at which the display changes. 
  int n;              // Number of menu primary items. (Set by the number of calls to addMenuItem).
  int p;              // Index to current menu item. (array subscript).
  char mode;          // Mode: 'o' = option, 'v' = value.

  // Variables for setting up menu structure in memory.  
  struct menu{
    char option[8];   // Menu option. 
    float value;      // The value associated with the menu option. 
    char access;      // Access permission: r = read, w = write.
    float delta;      // How much the value changes per button press.
  };

  menu selection[maxMenuSize]; // Array of structs. Max number of menu items is set by the constant maxMenuSize.

  float menuDefaults[maxMenuSize]; // Read only copy of menu values - used for resets.
};
#endif
