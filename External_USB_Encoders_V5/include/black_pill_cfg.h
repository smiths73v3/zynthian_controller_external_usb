/*******************************************************************************
 * STM32 Black Pill Clone configuration
 * 
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

  ------
  Boot Switch Run mode - Normal encoders
  |SW1| ON  |
  |SW2| OFF |
  |SW3| OFF |

  Boot Switch Run mode - Reversed encoders
  |SW1| ON  |
  |SW2| ON  |
  |SW3| OFF |

  Boot Switch Bootstrap mode
  |SW1| ON  |
  |SW2| OFF |
  |SW3| ON  |
  
  Board appears as "STM32 Bootloader" in Device Manager
  Here is a config that works in platformio.ini for this project:
        [env:genericSTM32F401CC]
        platform = ststm32
        board = genericSTM32F401CC
        framework = arduino
        build_flags = 
          -D PIO_FRAMEWORK_ARDUINO_ENABLE_HID
          -D USBCON
          -D HAL_PCD_MODULE_ENABLED
        lib_deps = mprograms/SimpleRotary@^1.1.3
        upload_protocol = dfu
*/

//LED Indicator pin 
#define led PC13

//Active HIGH or LOW encoders
#define r_polarity HIGH

//Normal rotation, or inverted encoders
#define pin_invert PB2
#define default_invert false

#define ENC0_GND  PA10
#define ENC0_VCC  PA9
#define ENC0_SW   PA8
#define ENC0_A    PA7
#define ENC0_B    PA6

#define ENC1_GND  PA4
#define ENC1_VCC  PA3
#define ENC1_SW   PA2
#define ENC1_A    PA1
#define ENC1_B    PA0

#define ENC2_GND  PB9
#define ENC2_VCC  PB10
#define ENC2_SW   PB12
#define ENC2_A    PB13
#define ENC2_B    PB14

#define ENC3_GND  PB3
#define ENC3_VCC  PB4
#define ENC3_SW   PB5
#define ENC3_A    PB6
#define ENC3_B    PB7

#define SW0       PA15
#define SW1       PA14
#define SW2       PA13
#define SW3       PB0
