/*
    FWMenu – Interactive Menu Interface for a TM1638 LED Module.
                  Copyright © 2015 Frank Williams.

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
*/


#include "Arduino.h"

/* Summary
   =======
 FWMenu.h - Library for creating an interactive menu for use with a TM1638 module.
 The user can specify menu options at compile time and modify their values at run time
 by using buttons on the TM1638 module.
 
 Created by Frank Williams, May 17, 2015.
 Released into the public domain.
*/

/**********************************************************************************************************************************
  TM1638 LED Display Menu Program
  ===============================
  
  This program allows a user to set up a table of menu items. 
  Each menu item (selection) consists of a name (called an option in the code) and a value. These can be displayed by the LED display unit.
  Each selection also consists of an access attribute (read or write) and a delta amount which is the increment used when modifying the value.
  The access attribute and the delta amount are only modifiable by changing the code.
  
  
  To use this, create a 'table' of menu selections by making multiple calls to addMenuItem() in the setup() method. 
  All amounts in the 'table' are floating point numbers with 1 decimal place.
  Ensure that the array called selections[] is big enough to hold all of your menu items.
  
  
  There are two modes used within FWMenu: 
  Mode='o' for option processing. 
  Mode='v' for value processing. 
  
  Buttons B2 and B3 work differently in each mode.
  
  
  Navigation:
  ===========
                Mode = 'o'             Mode = 'v'
                ----------             ----------
  Button 2 -    Go down the menu.      Decrease the value by the delta amount in the table.
  
  Button 3 -    Go up the menu.        Increase the value by the delta amount in the table. 
  
  Button 4 -    Switch to Mode='v'.    Switch to Mode='o'. This is the <Enter> key.
    
  Button 5 -    Clear the display.     Note: to get back to where you left off 
                                       press each of B2 and B3 once. 
                                       The order doesn't matter.
  
  Button 8 -    Reset menu to default
                values.  
*/

#include "TM1638.h"
#include "FWMenu.h"

// Constructor
FWMenu::FWMenu(byte dataPin, byte clockPin, byte strobePin, boolean activateDisplay, byte intensity)
:
TM1638(dataPin, clockPin, strobePin, activateDisplay, intensity){
}


//**********************************************************************************************************************************
void FWMenu::setupMenu(int pace){

  // Button number masks. 
  button1 = Btn1;   
  button2 = Btn2; 
  button3 = Btn3; 
  button4 = Btn4; 
  button5 = Btn5; 
  button6 = Btn6; 
  button7 = Btn7; 
  button8 = Btn8; 

  _pace = pace;   // Pace at which the menu changes - default 500 milliseconds.  
  n = 0;          // Number of menu primary items. (Set by number of calls to addMenuItem).
  p = 0;          // Index to current menu item.   (array subscript).

  // Set up LED display. .
  setupDisplay(true,7);

  // 'Option' mode to start. 
  mode = 'o';
}


// Set up menu storage.- 'Opt' is always 8 characters. 'val' has 1 decimal place always.  
void FWMenu::addToMenu(const char opt[],float val,char attr,float delta){
  for (int i=0;i<8;i++){
    selection[n].option[i]=opt[i];
  }
  selection[n].value=val;        // This is displayed with 1 decimal place only.
  selection[n].access=attr;
  selection[n].delta=delta;
  menuDefaults[n]=val;           // Copy of values for resets.            
  n++; 
}


void FWMenu::useMenu(){

  unsigned long value;          // To provide (long)selection[p].value.

  // Populate the buttonz private member variable.
  setButtonz(getButtons());   

  // Menu functionality - Display and set values.
  //=============================================
  // B1 - Spare at present.
  if (aButtonPressed(button1)){
    //Later this will be used to start a cycle.
  }

  // B2 - Up/--.
  if (aButtonPressed(button2)){
    if (mode=='o'){
      if (p>0){    	// Not at top?
        p--;       	// Move up 1 cell.
      }          
      String s = selection[p].option; 
      setDisplayToString(s);
    }
    if (mode=='v') {
      if (selection[p].access=='w') {
		// Don't allow a decrement below zero
		float testval = selection[p].value-selection[p].delta;
		if (testval>=0)
          selection[p].value-=selection[p].delta;                    
        // Display value.
		value = (unsigned long)(selection[p].value*10);
        setDisplayToDecNumber(value,2,false);
      }
    }
  }

  // B3 - Down/++.
  if (aButtonPressed(button3)){
    if (mode=='o'){
      if (p<n-1){  	// Not at bottom?
        p++;       	// Move down 1 cell.
      }       
      String s = selection[p].option; 
      setDisplayToString(s);
    }
    if (mode=='v') {
      if (selection[p].access=='w') {
        selection[p].value+=selection[p].delta;
        // Display value.
		value = (unsigned long)(selection[p].value*10);
        setDisplayToDecNumber(value,2,false);            // provided cast value here
      }
    }
  }

  // B4 - <Enter> and swap modes.
  if (aButtonPressed(button4)){
    if (mode=='o'){
      mode='v';
      // Display value.
	  value = (unsigned long)(selection[p].value*10);
      setDisplayToDecNumber(value,2,false);
    } 
    else
      if (mode=='v') {
        mode='o';
        // Display option.
        setDisplayToString(selection[p].option);
      }
  }

  // B5 - Clear display
  if (aButtonPressed(button5)){
	showDisplay = !showDisplay;
    if (!showDisplay){
		clearDisplay();
	}
	mode='o';  // We always 'switch on' (display something on the LED's) again in 'Option Mode'.
    // A little trick here - when you switch back on using B2 or B3, press each of these buttons once to return to where you were.
  }

  // B8 - Reset menu to default values
  if (aButtonPressed(button8)){
    resetMenu();
    setDisplayToString("DEFAULTS");
  }


  delay(_pace);   // How fast the display changes.  
}


/* Function aButtonPressed() - return 1 or 0 to indicate if a button on its own is pressed.
    So if the user presses say two buttons, even if one of them is the one we are testing for,
    the result will be false. */
boolean FWMenu::aButtonPressed(byte btn){ 
  byte temp = getButtonz();
  temp &= 0xff-btn;      // Switch off the wanted bit and see if any more are still on. 
  if (temp == 0){
     // Only 1 button was pressed. Now test it's value.
     temp = getButtonz();
     if (temp & btn)
         return 1;
     else
         return 0;
  }
  else
     return 0; // Multiple buttons were pressed so result is false.
}


// Make a menu value available. 
float FWMenu::getMenuValueForOption(const char opt[]){
  // Loop through menu.
  for (int i=0;i<n;i++){
    boolean match; 
    // Compare: option in menu vs option in parm. 
    for (int j=0;j<8;j++){

      if (selection[i].option[j]==opt[j])
        match = true;
      else {
        match = false;
        break;
      }

    } /* for j */


    // If we found a match return the corresponding value.        
    if (match==true){ 
      return selection[i].value;
    }

  } /* for i */

  // Error - Menu option supplied did not match any defined using addToMenu(). 
  // To fix - Check argument matches arg1 in addToMenu(), 8 characters.
  return -1;
}


// Reset menu values to defaults.
void FWMenu::resetMenu(){
  for (int i=0;i<n;i++){
    selection[i].value=menuDefaults[i]; 
  }
}
