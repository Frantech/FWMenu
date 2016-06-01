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

/* Example of how to use FWMenu the most simple way. 
   =================================================
   This provides a user interface allowing you to set up a menu at compile time and
   access or modify the values of the menu using the buttons at run time.
  
   Buttons 1,7,8 are available to be used for whatever you wish.
 
   For instructions on what the buttons do see the FWMenu.cpp file.   
   The menu demonstrated is a nonsense example but shows what can be done.
*/

// Create a menu object.
FWMenu m(50, 38, 40, true, 1);

//**********************************************************************************************************************************
void setup() {

  // COM output.
  Serial.begin(9600);
  Serial.println("--Setup--");

  // Prepare FWMenu object.   
  m.setupMenu();    // Pace is defaulted to 500 milliseconds. Supply an arg here if you want to slow the display down.

  // 'Table' used to set up menu storage. Arguments: Option, value, access permission, delta. Up to 16 Menu Lines can be defined.
  m.addToMenu("VERSION ", 1.0,'r',0.0);    // p=0 Software version (read only)
  m.addToMenu("LENGTH  ",1200.0,'w',1.0);  // p=1
  m.addToMenu("WIDTH   ",600.0,'w',10.0);  // p=2
  m.addToMenu("HEIGHT  ",15.0,'w',2.5);    // p=3
}

//**********************************************************************************************************************************
void loop() {
  Serial.println("--Loop--");

  // Process any button presses.
  m.useMenu();

  // Get hold of a menu value so we can use it.
  float f = m.getMenuValueForOption("HEIGHT  ");
  Serial.print("  HEIGHT Value obtained=");
  Serial.println(f);  

  // Now set up another button to do something - note buttons 1,6 and 7 are available.
  if (m.aButtonPressed(m.button7))
    Serial.println("Button 7 was pressed");

}


