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
*/


#include <TM1638.h>
#include <FWMenu.h>

/* Example of using FWMenu under an "Administrator" userMode. 
 ============================================================
 This would be good for when you want to configure a device at run time, but you do not want the users to be able to do this.
 (They'd need to know the button combination to get into Admin mode.)
 
 This example implements a menu structure that looks like the following diagram.
 The design allows you to toggle between user mode and admin mode.
 
    User mode   (LED8 Green)
       |
       |---B1+B3-- Admin mode
       |
       |---B2----- Put out a message
       |
       No other buttons are defined. B1,B3-B8 are available (as well as combinations). 
       
       
    Admin mode  (LED8 Red)
       |
       |---B1+B3-- User mode
       |
       |---FWMenu interface using B2,B3,B4,B5 and B8.
       |
       |---B7---- defined by user.  
       |
       No other buttons are defined. B1,B6 are available (as well as combinations). 
 
 
 
 This demonstrates how you can write code that uses a user mode which allows any of the buttons to do whatever you like.     
 
 Description of the user experience.
 ==================================
 When you start the software the rightmost LED (LED 8) glows green to indicate that you are in "User Mode".
 If you press B2, the COM interface reports that B2 has been pressed. 
 
 But to get down into FWMenu you need to press a key *combination*. (Button1 + Button3 in this case.).
 After that, FWMenu will function. FWMenu uses buttons 2,3,4 to navigate the menu and change values,  
 button 5 to clear the screen, and button 8 to reset default values.
 
 If you then press the combination (B1+B3) again you will be taken out of "Administrator" and back into "User" userMode.
 At this level, any button can be programmed to do whatever is required and menu values cannot be touched. They can of course be read by your software.
 */

// Create a menu object.
FWMenu m(50, 38, 40, true, 1);

char userMode;            //Admin or User. Admin gives access to FWmenu 

//**********************************************************************************************************************************
void setup() {

  // COM output.
  Serial.begin(9600);
  Serial.println("--Setup--");

  // Prepare FWMenu object.   
  m.setupMenu(700);    // Pace is defaulted to 500 milliseconds. Supply an arg here if you want to change the rate at which the display changes.

  // 'Table' used to set up menu storage. Arguments: Option, value, access permission, delta.
  m.addToMenu("VERSION ",1.0,'r',0.0);     // p=0 Software version (read only)
  m.addToMenu("LENGTH  ",1200.0,'w',1.0);  // p=1
  m.addToMenu("WIDTH   ",600.0,'w',10.0);  // p=2
  m.addToMenu("HEIGHT  ",15.0,'w',2.5);    // p=3

  userMode = 'u';                          // Default to User mode
  m.setLED(TM1638_COLOR_GREEN,7);          // User mode indicator
}

//**********************************************************************************************************************************
void loop() {
  Serial.println("--Loop--");

  // Populate the buttonz private member variable.
  m.setButtonz(m.getButtons());   

  // Check if we are going to toggle userModes. (Mode change) 
  if (m.getButtons() == m.button1 + m.button3){
    Serial.println("Combination - Both Button 1 and Button 3 were pressed");

    // Toggle the user mode and set mode indicator LED  
    if (userMode == 'u'){
      userMode = 'a';      // Admin mode
      m.setLED(TM1638_COLOR_RED,7);  
    }
    else {
      userMode = 'u';      // Normal user mode
      m.setLED(TM1638_COLOR_GREEN,7);  
    }
    Serial.print("User mode = ");
    Serial.println(userMode);
  }

  if ( userMode == 'a') {
    // Admin mode - use FWMenu
    //========================
    // Process any button presses.
    m.useMenu();

    // Get hold of a menu value so we can use it.
    float f = m.getMenuValueForOption("HEIGHT  ");         // Arg needs to be 8 characters.
        Serial.print("  HEIGHT Value obtained=");
    Serial.println(f);  

    // Now set up another button to do something - note buttons 1,6 and 7 are available. The others have already been used by FWMenu.
    if (m.aButtonPressed(m.button7))
      Serial.println("Button 7 was pressed");
  }

  else
  {
    // User mode
    //==========
    // All buttons are available for use here so we can cause them to do whatever we wish. e.g.
    if (m.aButtonPressed(m.button2))
      Serial.println("Button 2 was pressed in user mode this does nothing to the menu"); // In admin mode this would take you down the menu
 
    delay(700);    //We need a delay here because the loop delay is coded inside in m.useMenu. When in user mode that delay will not be invoked.
  }
}













