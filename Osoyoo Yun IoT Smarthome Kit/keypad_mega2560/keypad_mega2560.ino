#include <Console.h>
#include <Bridge.h>
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {47, 45, 43, 41}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {39, 37, 35, 33}; //connect to the column pinouts of the keypad
 
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
 
void setup(){
  Bridge.begin();
  Console.begin();
  while (!Console);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Console.println(customKey);
  }
}
