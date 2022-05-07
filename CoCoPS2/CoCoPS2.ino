
/*  CoCoPS/2
 *  Project info
 *  https://classiccomputerworkshop.com/coco-ps-2/
 *  Firmware and documentation
 *  https://github.com/classiccomputerworkshop/classiccomputerworkshop/tree/main/CoCoPS2
 *  Arduino Libraries
 *  https://github.com/rucek/arduino-ps2-mouse
 *  https://github.com/RobTillaart/MCP4725
 *  STLs for case
 *  https://www.thingiverse.com/thing:5353822
 *  This code is provided free of charge and you can do what you want with it.
*/
 
#include <PS2Mouse.h>
#include <MCP4725.h>

MCP4725 dac1(0x60);             // X axis DAC
MCP4725 dac2(0x61);             // Y axis DAC
PS2Mouse mouse(3,2);            // Assign PS/2 Mouse Pins
bool Button1, Button2;          // current button states
bool Button1State = false;      // Previous buttom 1 state
bool Button2State = false;      // Previous button 2 state
int xposition = 2048;           // Start Position
int yposition = 2048;           // Start Position
int multiplier;                 // Mouse scaller
unsigned long xsum = 0;         // x input accumulator
unsigned long ysum = 0;         // y input accumulator
  
void setup(){
  mouse.initialize();
  dac1.begin();
  dac2.begin();
  pinMode(4, INPUT_PULLUP);     // Mouse Multiplier selection
  pinMode(5, INPUT_PULLUP);     // Mouse Multiplier selection
  pinMode(6, INPUT_PULLUP);     // Select between Mouse (HIGH) and Joystick (LOW)
  pinMode(7, OUTPUT);           // Button 1 Output
  pinMode(8, OUTPUT);           // Button 2 Output
  pinMode(12, INPUT_PULLUP);    // Joy Button 1
  pinMode(11, INPUT_PULLUP);    // Joy Button 2
  pinMode(A0, INPUT);           // Joy y axis
  pinMode(A1, INPUT);           // Joy x axis
  if (digitalRead(4)) {if (digitalRead(5)) multiplier = 6; else multiplier = 5; }                  // Set Mouse Resolution Multiplier
    else { if (digitalRead(5)) multiplier = 2; else multiplier = 3;}                               
}                                                                                                   

void loop(){ 
  
  if (!dac1.isConnected())                                                                         // Verify DAC1 online 
    {dac1.begin(); dac2.begin(); delay(100);}                                                      // Attempt reconnect if needed
                                                        
///////////////////////////////////////////////////////Mouse Mode//////////////////////////////////////////////////////////////////////
 
 if (digitalRead(6)) {                                                                             // Pin 4 HIGH -> Mouse Mode
  MouseData data = mouse.readData();                                                               // Read mouse data
  if ((data.status & 0x1) == 1) Button1 = true; else Button1 = false;                              // Check for left button pressed
  if ((data.status & 0x2) == 2) Button2 = true; else Button2 = false;                              // Check for right button pressed
  xposition = (xposition + (data.position.x * multiplier));                                        // Add X Delta to X position
  if (xposition < 0) xposition = 0; else if (xposition > 4095) xposition = 4095;                   // Check for X position out of range  
  yposition = (yposition - (data.position.y * multiplier));                                        // Subtract Y Delta to Y position
  if (yposition < 0) yposition = 0; else if (yposition > 4095) yposition = 4095;                   // Check for Y position out of range  
  if (data.position.x != 0) dac1.writeDAC(xposition);                                              // Set new X Output if change
  if (data.position.y != 0) dac2.writeDAC(yposition);                                              // Set new Y Output if change
  if (Button1 != Button1State) { if (Button1) {digitalWrite(7, HIGH); Button1State = true;}        // Chamge state of left button
                                   else {digitalWrite(7, LOW); Button1State= false;}}
  if (Button2 != Button2State) { if (Button2) {digitalWrite(8, HIGH); Button2State = true;}        // Chamge state of right button
                                   else {digitalWrite(8, LOW); Button2State= false;}}
  delay(5);
///////////////////////////////////////////////////////Joystick Mode//////////////////////////////////////////////////////////////////
 } else {                                                                                          // Pin 4 LOW -> Joystick Mode
  xsum = 0; ysum = 0;                                                                              // Reset x and y accumulators
  for (int x = 0; x < 100; x++) {xsum = xsum + analogRead(1);ysum = ysum + analogRead(0);}         // Take 100 readings x pos and y pos
  xposition = xsum / 100;                                                                          // Average x readings
  yposition = ysum / 100;                                                                          // Average y readings
  xposition = 4*(1250000/xposition) - 5000;                                                        // Calculate X Position
  yposition = 4*(1250000/yposition) - 5000;                                                        // Calculate Y Position  
  if (xposition < 0) xposition = 0; else if (xposition > 4095) xposition = 4095;                   // Check for X position out of range
  if (yposition < 0) yposition = 0; else if (yposition > 4095) yposition = 4095;                   // Check for Y position out of range
  dac1.writeDAC(xposition);                                                                        // Output to X DAC
  dac2.writeDAC(yposition);                                                                        // Output to Y DAC
  Button1 = digitalRead(12);                                                                       // Read Joy Button1 State
  Button2 = digitalRead(11);                                                                       // Read Joy Button2 State
  if (Button1 != Button1State) { if (Button1) {digitalWrite(7, LOW); Button1State = true;}         // Chamge state of Joy Button 1
                                   else {digitalWrite(7, HIGH); Button1State= false;}}
  if (Button2 != Button2State) { if (Button2) {digitalWrite(8, LOW); Button2State = true;}         // Chamge state of Joy Button 2
                                   else {digitalWrite(8, HIGH); Button2State= false;}}
  
 }
   
}
