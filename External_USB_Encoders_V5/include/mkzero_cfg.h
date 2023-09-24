/*******************************************************************************
 * MKZERO configuration

|Encoder Configuration
| Pin | Control    | Device  | Comments |
|-----|------------|---------|---|
| D0  | Encoder    | Channel | - |
| D1  | Encoder    | Channel | - |
| D2  | Switch     | Channel | - |
| D3  | Encoder    | Back    | - |
| D4  | Encoder    | Back    | - |
| D5  | Switch     | Back    | - |
| D11 | Encoder    | L/S     | - |
| D6  | Encoder    | L/S     | Swapped with pin 10 for interrupt for encoder |
| D9  | Switch     | L/S     | - |
| D7  | Encoder    | Select  | - |
| D8  | Encoder    | Select  | - |
| D10 | Switch     | Select  | Swapped with pin 6 for interrupt for encoder |

|Front panel switches
| D13 | PB Switch  | 1 | - |
| D12 | PB Switch  | 2 | - |
| D20 | PB Switch  | 3 | - |
| D21 | PB Switch  | 4 | - |

***************************************************/

//LED Indicator pin 
#define led LED_BUILTIN

//Active HIGH or LOW encoders
#define r_polarity HIGH

//Normal rotation, or inverted encoders
#define pin_invert PIN_NA
#define default_invert false

#define ENC0_GND  PIN_NA
#define ENC0_VCC  PIN_NA
#define ENC0_SW   D5
#define ENC0_A    D3
#define ENC0_B    D4

#define ENC1_GND  PIN_NA
#define ENC1_VCC  PIN_NA
#define ENC1_SW   D2
#define ENC1_A    D1
#define ENC1_B    D0

#define ENC2_GND  PIN_NA
#define ENC2_VCC  PIN_NA
#define ENC2_SW   D9
#define ENC2_A    D6
#define ENC2_B    D11

#define ENC3_GND  PIN_NA
#define ENC3_VCC  PIN_NA
#define ENC3_SW   D10
#define ENC3_A    D7
#define ENC3_B    D8

#define SW0       D13
#define SW1       D12
#define SW2       D20
#define SW3       D21
