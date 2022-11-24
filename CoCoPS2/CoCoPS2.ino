/*
 *  CoCoMousePS/2
 *  PC Joystick and PS/2 mouse adapter for Tandy Color Computers
 *  https://classiccomputerworkshop.com
 *  https://github.com/classiccomputerworkshop/classiccomputerworkshop
 *  Free for all to enjoy.
 */


#include <PS2Mouse.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac1;                   // X axis DAC
Adafruit_MCP4725 dac2;                   // Y axis DAC

PS2Mouse mouse(3, 2);                    // Assign PS/2 Mouse Pins

bool Button1, Button2;
bool Button1State = false;
bool Button2State = false;
int xposition = 2048;                    // Start Position
int yposition = 2048;                    // Start Position
int multiplier;

void setup() {
  dac1.begin(0x60);                      // Set X axis DAC address
  dac2.begin(0x61);                      // Set Y axis DAC address
  pinMode(4, INPUT_PULLUP);              // Mouse Multiplier selection
  pinMode(5, INPUT_PULLUP);              // Mouse Multiplier selection
  pinMode(6, INPUT_PULLUP);              // Select between Mouse (HIGH) and Joystick (LOW)
  pinMode(7, OUTPUT);                    // Button 1 Output
  pinMode(8, OUTPUT);                    // Button 2 Output
  pinMode(12, INPUT_PULLUP);             // Joy Button 1
  pinMode(11, INPUT_PULLUP);             // Joy Button 2
  pinMode(A0, INPUT);                    // Joy y axis
  pinMode(A1, INPUT);                    // Joy x axis
  if (digitalRead(4)) {                  // Set Mouse Resolution Multiplier
    if (digitalRead(5)) multiplier = 6;  // 4 on  5 on  -> 6
    else multiplier = 5;                 // 4 on  5 off -> 5
  }                                      // 4 off 5 off -> 3
  else {                                 // 4 off 5 on  -> 2
    if (digitalRead(5)) multiplier = 2;
    else multiplier = 3;
  }
}

void loop() {
  ///////////////////////////////////////////////////////Mouse Mode//////////////////////////////////////////////////////////////////
  if (digitalRead(6)) {                                                                  // Pin 4 HIGH -> Mouse Mode
    MouseData data = mouse.readData();                                                   // Get data from mouse
    if ((data.status & 0x1) == 1) Button1 = true; else Button1 = false;                  // Check for left button pressed
    if ((data.status & 0x2) == 2) Button2 = true; else Button2 = false;                  // Check for right button pressed
    xposition = (xposition + (data.position.x * multiplier));                            // Add X Delta to X position
    if (xposition < 0) xposition = 0; else if (xposition > 4095) xposition = 4095;       // Check for X position out of range
    yposition = (yposition - (data.position.y * multiplier));                            // Subtract Y Delta to Y position
    if (yposition < 0) yposition = 0; else if (yposition > 4095) yposition = 4095;       // Check for Y position out of range
    if (data.position.x != 0) dac1.setVoltage(xposition, false);                         // Set new X Output if change
    if (data.position.y != 0) dac2.setVoltage(yposition, false);                         // Set new Y Output if change
    if (Button1 != Button1State) {                                                       // Chamge state of left button if needed
      if (Button1) {digitalWrite(7, HIGH); Button1State = true;}
      else {digitalWrite(7, LOW); Button1State = false;}
    }
    if (Button2 != Button2State) {                                                       // Chamge state of right button if needed
      if (Button2) {digitalWrite(8, HIGH); Button2State = true;}
      else {digitalWrite(8, LOW); Button2State = false;}
    }

    ///////////////////////////////////////////////////////Joystick Mode/////////////////////////////////////////////////////////////
  } else {                                                                               // Pin 4 LOW -> Joystick Mode
    xposition = analogRead(1);                                                           // Read Joy X Value
    yposition = analogRead(0);                                                           // Read Joy Y Value
    if (xposition) xposition = 4 * (1278750 / xposition - 1250); else xposition = -1;    // Calculate X Position
    if (yposition) yposition = 4 * (1278750 / yposition - 1250); else yposition = -1;    // Calculate Y Position                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ;
    if (xposition < 0) xposition = 0; else if (xposition > 4095) xposition = 4095;       // Check for X position out of range
    if (yposition < 0) yposition = 0; else if (yposition > 4095) yposition = 4095;       // Check for Y position out of range
    dac1.setVoltage(xposition, false);                                                   // Output to X DAC
    dac2.setVoltage(yposition, false);                                                   // Output to Y DAC
    Button1 = digitalRead(12);                                                           // Read Joy Button1 State
    Button2 = digitalRead(11);                                                           // Read Joy Button2 State
    if (Button1 != Button1State) {                                                       // Chamge state of Joy Button 1 if needed
      if (Button1) {digitalWrite(7, LOW); Button1State = true;}
      else {digitalWrite(7, HIGH); Button1State = false;}
    }
    if (Button2 != Button2State) {                                                       // Chamge state of Joy Button 2 if needed
      if (Button2) {digitalWrite(8, LOW); Button2State = true;}
      else {digitalWrite(8, HIGH); Button2State = false;}
    }
  }
}
