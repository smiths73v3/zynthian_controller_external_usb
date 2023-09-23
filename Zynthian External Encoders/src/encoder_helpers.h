/***************************************************************
 * Constatnts
 */
#define KEY_NONE 0x0

//Switch press times in ms according to the zynthian UI Users Guide
#define sw_short 1
#define sw_bold  300
#define sw_long  2000

#define PIN_NA   999 /* Indicate HW pin not used */
/**************************************************************
 * Typedefs
 */
typedef struct KeyMap_s {
  char key_cw;
  char key_ccw;
  char mod1_enc;
  char mod2_enc;
  char key_switch;
  char mod_short;
  char mod_bold;
  char mod_long;
}KeyMap_t;

/**************************************************************
 * Global Variables
 */
byte cw = 1;
byte ccw = 2;
boolean invert_encoders = false;

/**************************************************************
 * Macros
 * Uses name magic so we can do things like
 * ENCODER_CREATE(select) and get everything set up.
 */
#define ENCODER_CREATE(enc_name, enc_cw, enc_ccw, enc_mod1, enc_mod2, enc_sw, short_mod, bold_mod, long_mod ) \
                                 SimpleRotary r_##enc_name(r_##enc_name##_a, r_##enc_name##_b, r_##enc_name##_sw); \
                                 int r_##enc_name##_sw_time; \
                                 boolean r_##enc_name##_lp; \
                                 KeyMap_t r_##enc_name##_map = { enc_cw, enc_ccw, enc_mod1, enc_mod2, enc_sw, short_mod, bold_mod, long_mod };

//Encoders (also uses 2 GPIO pins per for GND/V+) and skip a pin to allow for JST style connectors?
#define ENCODER_DEF_PIN_MAP(enc_name,enc_gnd,enc_vcc,enc_sw,enc_a,enc_b)  \
        int r_##enc_name##_gnd=enc_gnd; int r_##enc_name##_vcc=enc_vcc; \
        int r_##enc_name##_sw=enc_sw; int r_##enc_name##_a=enc_a; int r_##enc_name##_b=enc_b;
                                  
#define ENCODER_SET_GPIO(enc_name) { if(r_##enc_name##_gnd != PIN_NA) { pinMode(r_##enc_name##_gnd, OUTPUT); digitalWrite(r_##enc_name##_gnd, 0); }\
                                     if(r_##enc_name##_gnd != PIN_NA) { pinMode(r_##enc_name##_vcc, OUTPUT); digitalWrite(r_##enc_name##_vcc, 1); }\
                                     r_##enc_name.setTrigger(r_polarity); }while(0)

#define ENCODER_PROCESS(enc_name) encoder_process(&r_##enc_name,&r_##enc_name##_sw_time,&r_##enc_name##_lp,&r_##enc_name##_map)

#define KEY_PRESS(k_p)   { if(k_p) Keyboard.press(k_p); }while(0)      
#define KEY_RELEASE(k_p) { if(k_p) Keyboard.release(k_p); }while(0)      

/* If Caps lock, repress to make sure it's "released" for for other keys */
#define COMBO_KEY(key,mod1, mod2) { KEY_PRESS(mod1); KEY_PRESS(mod2); KEY_PRESS(key); Keyboard.releaseAll(); if((mod1 == KEY_CAPS_LOCK)||(mod2 == KEY_CAPS_LOCK)){ KEY_PRESS(KEY_CAPS_LOCK); Keyboard.releaseAll(); } }while(0)
#define COMBO_PRESS(key,mod1,mod2) { KEY_PRESS(mod1); KEY_PRESS(mod2); KEY_PRESS(key); }while(0)
#define COMBO_RELEASE(key,mod1,mod2) { KEY_RELEASE(key); KEY_RELEASE(mod2); KEY_RELEASE(mod1); }while(0)


/******************************************************************
 * Buttons Macros with name magic
 */
#define BUTTON_CREATE(btn_name,btn_gpio,btn_sw,short_mod,bold_mod,long_mod) \
                        char b_##btn_name##_gpio = btn_gpio; \
                        int b_##btn_name##_sw_start; \
                        int b_##btn_name##_sw_time; \
                        boolean b_##btn_name##_lp; \
                        KeyMap_t b_##btn_name##_map = { 0, 0, 0, btn_sw, short_mod, bold_mod, long_mod };

#define BUTTON_SET_GPIO(btn_name) { if( b_##btn_name##_gpio != PIN_NA) { pinMode(b_##btn_name##_gpio, INPUT_PULLUP); } }while(0)

#define BUTTON_PROCESS(btn_name) button_process(&b_##btn_name##_map,b_##btn_name##_gpio,&b_##btn_name##_sw_start,&b_##btn_name##_sw_time,&b_##btn_name##_lp)

void press_raw(KeyMap_t *k_map, int sw, int * sw_pending, boolean * long_press);
void press_process(KeyMap_t *k_map, int sw, int * sw_pending, boolean * long_press);

/******************************************************************
 * Procedures
 */
int btn_pushTime(int pin, int * sw_start) { 
  int sw = 0;
  if( (pin != PIN_NA) && !digitalRead(pin) ) {
    if( *sw_start == 0 ) {
      *sw_start = millis(); //starting tick of button press
    }
    sw = millis() - *sw_start;
  }else
  {
    //button was released. calculate elapsed time, and reset start
    if( *sw_start ){
      sw = millis() - *sw_start;
      *sw_start = 0;
    }
  }
  return sw;
}
/***************************************************
 * encoder process
 * Read and handle input from the encoder
 */
void encoder_process(SimpleRotary *encoder,int *sw_pending, boolean *long_press, KeyMap_t *k_map) {
  byte enc;
  int sw;

  //Collect data out of the encoder
  enc = encoder->rotate();
  sw = encoder->pushTime();
  
  if( enc == cw )
  {
    COMBO_KEY(k_map->key_cw,k_map->mod1_enc,k_map->mod2_enc);
  }else if ( enc == ccw )
  {
    COMBO_KEY(k_map->key_ccw,k_map->mod1_enc,k_map->mod2_enc);
  }
#if( defined(V5_BEHAVIOR) )
  press_raw( k_map, sw, sw_pending, long_press);
#else
  press_process( k_map, sw, sw_pending, long_press);
#endif
}

/***************************************************
 * button process
 * Read and handle input from the 4 button interface
 */
void button_process(KeyMap_t *b_map, char pin, int * sw_start, int * sw_pending, boolean * long_press) {
  int sw;

  sw = btn_pushTime(pin,sw_start);
  press_process( b_map, sw, sw_pending, long_press );
}

/************************************************************************
 * press_raw ( map, current, elapsed, long_press)
 */
void press_raw(KeyMap_t *k_map, int sw, int * sw_pending, boolean * long_press) {
    
  /********** Long press, does not require release ***************/
  if( *sw_pending > sw_long && !*long_press ) {
    COMBO_KEY(k_map->key_switch, k_map->mod_long, KEY_NONE);
    *long_press = true;
  }

  if ( !sw && *sw_pending )
  {
    //Button is released, what was its longest value?
    if( *sw_pending > sw_long ) {
      *long_press = false; //Already sent the keys, just clear the event
    }else if ( *sw_pending > sw_bold ) {
      /************ Bold Press ************/
    COMBO_KEY(k_map->key_switch, k_map->mod_bold, KEY_NONE);
    }else if ( *sw_pending > sw_short) {
      /*********** Short Press ************/
    COMBO_KEY(k_map->key_switch, k_map->mod_short, KEY_NONE);
    }
  }
  *sw_pending = sw;
}

/************************************************************************
 * press_process ( map, current, elapsed, long_press)
 */
void press_process(KeyMap_t *k_map, int sw, int * sw_pending, boolean * long_press) {
    
  /********** Long press, does not require release ***************/
  if( *sw_pending > sw_long && !*long_press ) {
    COMBO_KEY(k_map->key_switch, k_map->mod_long, KEY_NONE);
    *long_press = true;
  }

  if ( !sw && *sw_pending )
  {
    //Button is released, what was its longest value?
    if( *sw_pending > sw_long ) {
      *long_press = false; //Already sent the keys, just clear the event
    }else if ( *sw_pending > sw_bold ) {
      /************ Bold Press ************/
    COMBO_KEY(k_map->key_switch, k_map->mod_bold, KEY_NONE);
    }else if ( *sw_pending > sw_short) {
      /*********** Short Press ************/
    COMBO_KEY(k_map->key_switch, k_map->mod_short, KEY_NONE);
    }
  }
  *sw_pending = sw;
}
