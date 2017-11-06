/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *Now let’s see how to print the key presses on an LCD. 
 *4X4 keypads use 8 pins . That takes up a lot of pins, 
 *so I’m going to use an I2C enabled LCD because it 
 *only needs 4 wires to connect to the Arduino.
 * Tutorial URL http://osoyoo.com/2017/09/13/arduino-lesson-4x4-matrix-keypad/
 * CopyRight www.osoyoo.com
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup(){
  lcd.backlight();
  lcd.init(); 
}

void loop(){
  char customKey = customKeypad.getKey();
  if (customKey){
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print(customKey);
  }
}

