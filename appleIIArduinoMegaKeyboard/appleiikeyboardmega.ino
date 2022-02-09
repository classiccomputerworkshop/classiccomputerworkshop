#include <Keypad.h>
#include <ctype.h>

// ClassicComputerWorkshop.com

const byte ROWS = 5; 
const byte COLS = 11; 
//define the symbols on the buttons of the keypad
byte numberKeys[ROWS][COLS] = {
  {0x03,0x04,0x05,0x00,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C},
  {0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17},
  {0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x02,0x21},
  {0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x01,0x00},
  {0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0x34,0x35},
};

byte rowPins[ROWS] = {12, 15, 14, 17, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {19, 18, 21, 20, 23, 22, 25, 24, 27, 30, 31}; //connect to the column pinouts of the keypad
byte key;
byte holdKey;
unsigned long t_hold;

Keypad numpad( makeKeymap(numberKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );  // Create new keypad

void setup() {
    numpad.begin( makeKeymap(numberKeys) );
    numpad.setHoldTime(600);   // Set hold time for keyboard
    pinMode(26, OUTPUT);       // Shift and Function keys, change to ground next revision
    digitalWrite(26, LOW);     // Shift and Function keys
    DDRL=B11111111;            // set port L to output for the keyboard output to computer
    DDRF=B11111111;            // strobe output
    DDRK=B11111100;            // shift/control input
    PORTK=B00000011;           // ebable pullup on shift and cntrl
}

void loop() {
   key = numpad.getKey();
   if (key){
    holdKey = key;
    switch (key) {                                        // Determine key and write to port
    case 0x02: PORTL = 0xA0 ;                             // space
                break;
    case 0x07: PORTL = 0x88 ;                             // right arrow BS
                break;
    case 0x0C: PORTL = 0x95 ;                             // left arrow (rubout)
                break;
    case 0x0A: PORTL = 0x8D ;                             // return
                break;
    case 0x8D: PORTL = 0x8D ;                             // lf function as return for now
                break;
    case 0x21: PORTL = 0x9B ;                             // escape
                break;
    case 0x35: PORTL = 0xDC ;                             // \ 
                break;
    case 0x34: PORTL = 0xDE ;                             // ^ 
                break;
    case 0x06: if ((PINK&0x01) != 0) { PORTL = 0xC0 ; }   // @
                  else               { PORTL = 0xFE ; }   // ~  
                break; 
    case 0x10: if ((PINK&0x01) != 0) { PORTL = 0xAC ; }   // ,
                  else               { PORTL = 0xBC ; }   // <
                break;
    case 0x01: if ((PINK&0x01) != 0) { PORTL = 0xAD ; }   // -
                  else               { PORTL = 0xBD ; }   // =
                break;
    case 0x0F: if ((PINK&0x01) != 0) { PORTL = 0xAE ; }   // .
                  else               { PORTL = 0xBE ; }   // >
                break;
    case 0x0E: if ((PINK&0x01) != 0) { PORTL = 0xBF ; }   // /
                  else               { PORTL = 0xAF ; }   // ?
                break;
    case 0x03: PORTL = 0xB0 ;                             // 0
                break;  
    case 0x33: if ((PINK&0x01) != 0) { PORTL = 0xB1 ; }   // 1
                  else               { PORTL = 0xA1 ; }   // !
                break;
    case 0x32: if ((PINK&0x01) != 0) { PORTL = 0xB2 ; }   // 2
                  else               { PORTL = 0xA2 ; }   // "
                break;
    case 0x31: if ((PINK&0x01) != 0) { PORTL = 0xB3 ; }   // 3
                  else               { PORTL = 0xA3 ; }   // #
                 break;
    case 0x30: if ((PINK&0x01) != 0) { PORTL = 0xB4 ; }   // 4
                  else               { PORTL = 0xA4 ; }   // $
                  break;
    case 0x2F: if ((PINK&0x01) != 0) { PORTL = 0xB5 ; }   // 5
                  else               { PORTL = 0xA5 ; }   // %
                break;
    case 0x2E: if ((PINK&0x01) != 0) { PORTL = 0xB6 ; }   // 6
                  else               { PORTL = 0xA6 ; }   // &
                break;  
    case 0x2D: if ((PINK&0x01) != 0) { PORTL = 0xB7 ; }   // 7
                  else               { PORTL = 0xA7 ; }   // '
                break;
    case 0x2C: if ((PINK&0x01) != 0) { PORTL = 0xB8 ; }   // 8
                  else               { PORTL = 0xA8 ; }   // (
                break;
    case 0x2B: if ((PINK&0x01) != 0) { PORTL = 0xB9 ; }   // 9
                  else               { PORTL = 0xA9 ; }   // )
                break;
    case 0x04: if ((PINK&0x01) != 0) { PORTL = 0xBA ; }   // :
                  else               { PORTL = 0xAA ; }   // *
                break;
    case 0x0D: if ((PINK&0x01) != 0) { PORTL = 0xBB ; }   // ;
                  else               { PORTL = 0xAB ; }   // +
                break;
    case 0x09: if ((PINK&0x01) != 0) { PORTL = 0xDD ; }   // ]
                  else               { PORTL = 0xFD ; }   // }
                break;
    case 0x08: if ((PINK&0x01) != 0) { PORTL = 0xDB ; }   // [
                  else               { PORTL = 0xFB ; }   // {
                break;
    case 0x20: if ((PINK&0x02) != 0) { PORTL = 0xC1 ; }   // A 
                  else               { PORTL = 0x81 ; }   // A cntrl
                break;
    case 0x13: if ((PINK&0x02) != 0) { PORTL = 0xC2 ; }   // B
                  else               { PORTL = 0x82 ; }   // B cntrl
                break;
    case 0x15: if ((PINK&0x02) != 0) { PORTL = 0xC3 ; }   // C
                  else               { PORTL = 0x83 ; }   // C cntrl
                break;
    case 0x1E: if ((PINK&0x02) != 0) { PORTL = 0xC4 ; }   // D
                  else               { PORTL = 0x84 ; }   // D cntrl
                break;
    case 0x28: if ((PINK&0x02) != 0) { PORTL = 0xC5 ; }   // E
                  else               { PORTL = 0x85 ; }   // E cntrl
                break;
    case 0x1D: if ((PINK&0x02) != 0) { PORTL = 0xC6 ; }   // F
                  else               { PORTL = 0x86 ; }   // F cntrl
                break;
    case 0x1C: if ((PINK&0x02) != 0) { PORTL = 0xC7 ; }   // G
                  else               { PORTL = 0x87 ; }   // G cntrl
                break;
    case 0x1B: if ((PINK&0x02) != 0) { PORTL = 0xC8 ; }   // H
                  else               { PORTL = 0x88 ; }   // H cntrl
                break;
    case 0x23: if ((PINK&0x02) != 0) { PORTL = 0xC9 ; }   // I
                  else               { PORTL = 0x89 ; }   // I cntrl
                break;
    case 0x1A: if ((PINK&0x02) != 0) { PORTL = 0xCA ; }   // J
                  else               { PORTL = 0x8A ; }   // J cntrl
                break;
    case 0x19: if ((PINK&0x02) != 0) { PORTL = 0xCB ; }   // K
                  else               { PORTL = 0x8B ; }   // K cntrl
                break;
    case 0x18: if ((PINK&0x02) != 0) { PORTL = 0xCC ; }   // L
                  else               { PORTL = 0x8C ; }   // L cntrl
                break;
    case 0x11: if (((PINK&0x01) != 0) && ((PINK&0x02) != 0))         { PORTL = 0xCD ; }   // M
                  else if (((PINK&0x01) == 0) && ((PINK&0x02) == 0)) { PORTL = 0x9D ; }   // M shift and cntrl
                  else if ((PINK&0x01) == 0)                         { PORTL = 0xDD ; }   // M shift
                          else                                       { PORTL = 0x8D ; }   // M cntrl
                break;
    case 0x12: if (((PINK&0x01) != 0) && ((PINK&0x02) != 0))         { PORTL = 0xCE ; }   // N
                  else if (((PINK&0x01) == 0) && ((PINK&0x02) == 0)) { PORTL = 0x9E ; }   // N shift and cntrl
                  else if ((PINK&0x01) == 0)                         { PORTL = 0xDE ; }   // N shift
                          else                                       { PORTL = 0x8E ; }   // N cntrl
                break;
    case 0x22: if ((PINK&0x02) != 0) { PORTL = 0xCF ; }   // O
                  else               { PORTL = 0x8F ; }   // O cntrl
                break;
    case 0x05: if (((PINK&0x01) != 0) && ((PINK&0x02) != 0))         { PORTL = 0xD0 ; }   // P
                  else if (((PINK&0x01) == 0) && ((PINK&0x02) == 0)) { PORTL = 0x80 ; }   // P shift and cntrl
                  else if ((PINK&0x01) == 0)                         { PORTL = 0xC0 ; }   // P shift
                          else                                       { PORTL = 0x90 ; }   // P cntrl
                break;
    case 0x2A: if ((PINK&0x02) != 0) { PORTL = 0xD1 ; }   // Q
                  else               { PORTL = 0x91 ; }   // Q cntrl
                break;
    case 0x27: if ((PINK&0x02) != 0) { PORTL = 0xD2 ; }   // R
                  else               { PORTL = 0x92 ; }   // R cntrl
                break;
    case 0x1F: if ((PINK&0x02) != 0) { PORTL = 0xD3 ; }   // S
                  else               { PORTL = 0x93 ; }   // S cntrl
                break;
    case 0x26: if ((PINK&0x02) != 0) { PORTL = 0xD4 ; }   // T
                  else               { PORTL = 0x94 ; }   // T cntrl
                break;
    case 0x24: if ((PINK&0x02) != 0) { PORTL = 0xD5 ; }   // U
                  else               { PORTL = 0x95 ; }   // U cntrl
                break;
    case 0x14: if ((PINK&0x02) != 0) { PORTL = 0xD6 ; }   // V
                  else               { PORTL = 0x96 ; }   // V cntrl
                break;
    case 0x29: if ((PINK&0x02) != 0) { PORTL = 0xD7 ; }   // W
                  else               { PORTL = 0x97 ; }   // W cntrl
                break;
    case 0x16: if ((PINK&0x02) != 0) { PORTL = 0xD8 ; }   // X
                  else               { PORTL = 0x98 ; }   // X cntrl
                break;
    case 0x25: if ((PINK&0x02) != 0) { PORTL = 0xD9 ; }   // Y
                  else               { PORTL = 0x99 ; }   // Y cntrl
                break;
    case 0x17: if ((PINK&0x02) != 0) { PORTL = 0xDA ; }   // Z
                  else               { PORTL = 0x9A ; }   // Z cntrl
                break;
      }   
     PORTF = PORTF | 0x04;  // strobe
     delayMicroseconds(5);  // delay for strobe
     PORTF = PORTF & 0xFB;  // strobe clear
     }
 
   if (numpad.getState() == HOLD) {                 // Detect key hold
      if ((millis() - t_hold) > 100 ) {
          PORTF = PORTF | 0x04;  // strobe
          delayMicroseconds(5);  // delay for stobe
          PORTF = PORTF & 0xFB;  // clear strobe
          t_hold = millis();
      }
   }    
}
