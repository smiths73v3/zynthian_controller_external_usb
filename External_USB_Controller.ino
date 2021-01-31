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
  
  WARNING:  PA12 and PA11 are USB DP/DN pins - can't be used when USB is in use.
  PA14 - SWCLK
  PA13 - SWDIO
  PA12 - USB DP
  PA11 - USB DN
  PB2  -  Bootloader? Config
  PB11 - Not wired to Edge 
  PC13 - BLUE LED

  Enc 1: PA6, PA7, PA8, PA9, PA10
  PA5 - SKIP
  Enc 2: PA0, PA1, PA2, PA3, PA4
  
  Enc 3: PB9, PB10, PB12, PB13, PB14
  PB8 - SKIP
  Enc 4: PB3, PB4, PB5, PB6, PB7

  Btn 1: PA15 z,shift,ctrl
  Btn 2: PA14 x,shift,ctrl
  Btn 3: PA13 c,shift,ctrl
  Btn 4: PB0  v,shift,ctrl
  
*/
#include <SimpleRotary.h>
#include <Keyboard.h>
#include "encoder_helpers.h"

//**********************************
//* Configuration stuffs
//**********************************

//Indicator LED
#define led PC13

//Normal rotation, or inverted encoders
#define pin_invert PB2

/***********************************************
 * Create the encoder instances and maps
 * This mapping is intended for an
 * STM32 Black Pill clone 
 */
ENCODER_DEF_PIN_MAP(back,PA10,PA9,PA8,PA7,PA6);
//ENCODER_CREATE(enc_name, enc_cw, enc_ccw, enc_mod, enc_sw, short_mod, bold_mod, long_mod )
ENCODER_CREATE(back, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_CAPS_LOCK, KEY_ESC, KEY_NONE, KEY_LEFT_SHIFT, KEY_LEFT_CTRL );
//ENCODER_DEF_PIN_MAP(enc_name,enc_gnd,enc_vcc,enc_sw,enc_a,enc_b)

ENCODER_DEF_PIN_MAP(layer,PA4,PA3,PA2,PA1,PA0);
ENCODER_CREATE(layer, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_SHIFT, 'l', KEY_NONE, KEY_LEFT_SHIFT, KEY_LEFT_CTRL );

ENCODER_DEF_PIN_MAP(snap,PB9,PB10,PB12,PB13,PB14);
ENCODER_CREATE(snap, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_CTRL, 's', KEY_NONE, KEY_LEFT_SHIFT, KEY_LEFT_CTRL );

ENCODER_DEF_PIN_MAP(select,PB3,PB4,PB5,PB6,PB7);
ENCODER_CREATE(select, KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_NONE, KEY_RETURN, KEY_NONE, KEY_LEFT_SHIFT, KEY_LEFT_CTRL );
/***********************************************/

/***********************************************
 * Create the button instances and maps
 */
BUTTON_CREATE(v4b_1,PA15,'z',KEY_NONE,KEY_LEFT_SHIFT,KEY_LEFT_CTRL);
BUTTON_CREATE(v4b_2,PA14,'x',KEY_NONE,KEY_LEFT_SHIFT,KEY_LEFT_CTRL);
BUTTON_CREATE(v4b_3,PA13,'c',KEY_NONE,KEY_LEFT_SHIFT,KEY_LEFT_CTRL);
BUTTON_CREATE(v4b_4,PB0 ,'v',KEY_NONE,KEY_LEFT_SHIFT,KEY_LEFT_CTRL);

/***************************************************
 * setup
 */
void setup() {
  // declare led pin to be an output:
  pinMode(led, OUTPUT);
  digitalWrite(led,0); //Turn on the LED to indicate that we are running code, not bootloader

  ENCODER_SET_GPIO(back);
  ENCODER_SET_GPIO(layer);
  ENCODER_SET_GPIO(snap);
  ENCODER_SET_GPIO(select);
  
  BUTTON_SET_GPIO(v4b_1);
  BUTTON_SET_GPIO(v4b_2);
  BUTTON_SET_GPIO(v4b_3);
  BUTTON_SET_GPIO(v4b_4);

  //Detect config for inverted rotation encoders
  pinMode(pin_invert, INPUT);
  if( digitalRead(pin_invert) ) {
    cw = 2;
    ccw = 1;
  }

  // wait for 2 second before starting keyboard.
  delay(2000);

// initialize control over the keyboard:
  Keyboard.begin();

  // wait for .5 second AFTER starting keyboard.
  delay(500);

  digitalWrite(led,1); //Turn off the LED once we are booted and servicing Encoders
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

/*******************************************************************
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
 
 */
