/***************************************************************
  ZYNTHIAN PROJECT: Aruino HID Keyboard controller
  
  This program takes 4 Arduino style rotary encoders
  and 4 independent push buttons and implements
  a HID keyboard mapped to the Standard Zynthian
  kepresses for each.
  This will allow for a portable Encoder/PB interface
  to a zynthian.
  (C)2020 SMITHS.73v3
*****************************************************************
                                                                            
 This program is free software; you can redistribute it and/or               
 modify it under the terms of the GNU General Public License as              
 published by the Free Software Foundation; either version 2 of              
 the License, or any later version.                                          
                                                                             
 This program is distributed in the hope that it will be useful,             
 but WITHOUT ANY WARRANTY; without even the implied warranty of              
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                
 GNU General Public License for more details.                                
                                                                             
 For a full copy of the GNU General Public License see the LICENSE.txt file. 
*******************************************************************                                                                             
  w/ Simple Rotary 1.1.2 Library from mprograms
*******************************************************************
 * Keymappings for reference - Oct 25th 2020
 Back Down    - "down" "Caps Lock"
 Back Up      - "up"   "Caps Lock"
 Back Short   - "esc" 
 Back Bold    - "esc"  "Shift"
 Back Long    - "esc"  "Ctrl"

 Layer Down   - "down" "Shift" 
 Layer Up     - "up"   "Shift"
 Layer Short  - "l" 
 Layer Bold   - "l"    "Shift" 
 Layer Long   - "l"    "Ctrl"

 Snap Down    - "down" "Ctrl"
 Snap Up      - "up"   "Ctrl"
 Snap Short   - "s" 
 Snap Bold    - "s"    "Shift" 
 Snap Long    - "s"    "Ctrl"

 Select Down  - "down" 
 Select Up    - "up" 
 Select Short - "ret" 
 Select Bold  - "ret"  "Shift" 
 Select Long  - "ret"  "Ctrl"

 S1           - "z"
 S2           - "x"    "Shift"
 S3           - "c"    "Ctrl"
 S4           - "v"    "Caps Lock"
 
********************************************************/
#include <SimpleRotary.h>
#include <Keyboard.h>
#include "encoder_helpers.h"

/**********************************
 * Include ONE of the following Hardware configurations
 * black_pill_cfg.h
 * mkzero_cfg.h
 */
//#include "hw_template_cfg.h"
#include "black_pill_cfg.h"
//#include "mkzero_cfg.h"

/***********************************************
 * Create the encoder instances and maps
 */
ENCODER_DEF_PIN_MAP(back,ENC0_GND,ENC0_VCC,ENC0_SW,ENC0_A,ENC0_B);
ENCODER_CREATE(back, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_CAPS_LOCK, KEY_ESC, KEY_NONE, KEY_LEFT_SHIFT, KEY_LEFT_CTRL );

ENCODER_DEF_PIN_MAP(layer,ENC1_GND,ENC1_VCC,ENC1_SW,ENC1_A,ENC1_B);
ENCODER_CREATE(layer, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_SHIFT, 'l', KEY_NONE, KEY_LEFT_SHIFT, KEY_LEFT_CTRL );

ENCODER_DEF_PIN_MAP(snap,ENC2_GND,ENC2_VCC,ENC2_SW,ENC2_A,ENC2_B);
ENCODER_CREATE(snap, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_CTRL, 's', KEY_NONE, KEY_LEFT_SHIFT, KEY_LEFT_CTRL );

ENCODER_DEF_PIN_MAP(select,ENC3_GND,ENC3_VCC,ENC3_SW,ENC3_A,ENC3_B);
ENCODER_CREATE(select, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_NONE, KEY_RETURN, KEY_NONE, KEY_LEFT_SHIFT, KEY_LEFT_CTRL );
/***********************************************/

/***********************************************
 * Create the button instances and maps
 */
BUTTON_CREATE(v4b_1,SW0,'z',KEY_NONE,KEY_LEFT_SHIFT,KEY_LEFT_CTRL);
BUTTON_CREATE(v4b_2,SW1,'x',KEY_NONE,KEY_LEFT_SHIFT,KEY_LEFT_CTRL);
BUTTON_CREATE(v4b_3,SW2,'c',KEY_NONE,KEY_LEFT_SHIFT,KEY_LEFT_CTRL);
BUTTON_CREATE(v4b_4,SW3,'v',KEY_NONE,KEY_LEFT_SHIFT,KEY_LEFT_CTRL);

/***************************************************
 * setup
 */
void setup() {
  // declare led pin to be an output:
  if( led != PIN_NA) { 
    pinMode(led, OUTPUT); 
    digitalWrite(led,0); //Turn on the LED to indicate that we are running code, not bootloader
  }

  ENCODER_SET_GPIO(back);
  ENCODER_SET_GPIO(layer);
  ENCODER_SET_GPIO(snap);
  ENCODER_SET_GPIO(select);
  
  BUTTON_SET_GPIO(v4b_1);
  BUTTON_SET_GPIO(v4b_2);
  BUTTON_SET_GPIO(v4b_3);
  BUTTON_SET_GPIO(v4b_4);

  //Detect config for inverted rotation encoders
  invert_encoders = default_invert;
  if( pin_invert != PIN_NA ) {
    pinMode(pin_invert, INPUT);
    if( digitalRead(pin_invert) ) {
      invert_encoders = true;
    }
  }
  if( invert_encoders ) {
      cw = 2;
      ccw = 1;
  }    

  // wait for 2 second before starting keyboard.
  delay(2000);

// initialize control over the keyboard:
  Keyboard.begin();

  // wait for .5 second AFTER starting keyboard.
  delay(500);

  if( led != PIN_NA) { digitalWrite(led,1); } //Turn off the LED once we are booted and servicing Encoders
}

/***************************************************
 * loop
 */
void loop() {  
  ENCODER_PROCESS(back);
  ENCODER_PROCESS(layer);
  ENCODER_PROCESS(snap);
  ENCODER_PROCESS(select);
  
  BUTTON_PROCESS(v4b_1);
  BUTTON_PROCESS(v4b_2);
  BUTTON_PROCESS(v4b_3);
  BUTTON_PROCESS(v4b_4);
}
