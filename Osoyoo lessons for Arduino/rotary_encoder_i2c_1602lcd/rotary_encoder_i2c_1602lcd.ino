/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *In this example, we will show the count of the encoder 
 *on the I2C 1602 LCD display.
 * Tutorial URL http://osoyoo.com/?p=11509
 * CopyRight www.osoyoo.com
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD 
//address to 0x27 for a 16 chars and 2 line display
const int clkPin= 2; //the clk attach to pin2
const int dtPin= 3; //the dt attach to pin3
const int swPin= 4 ;//the number of the button
int encoderVal = 0;

void setup()
{ 
  lcd.init();  // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("The value is :");
  //set clkPin,dePin,swPin as INPUT
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);
  Serial.begin(9600); // initialize serial communications at 9600 bps
}

void loop()
{
  int change = getEncoderTurn();
  encoderVal = encoderVal + change;
  if(digitalRead(swPin) == LOW)//if button pull down
  {
    encoderVal = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("The value is :");
  }
  Serial.println(encoderVal); //print the encoderVal on the serial monitor
  lcd.setCursor(5,1);
  lcd.print(encoderVal);
}

int getEncoderTurn(void)
{
  static int oldA = HIGH; //set the oldA as HIGH
  static int oldB = HIGH; //set the oldB as HIGH
  int result = 0;
  int newA = digitalRead(dtPin);//read the value of clkPin to newA
  int newB = digitalRead(clkPin);//read the value of dtPin to newB
  if (newA != oldA || newB != oldB) //if the value of clkPin or the dtPin has changed
  {
    // something has changed
    if (oldA == HIGH && newA == LOW)
    {
      result = (oldB * 2 - 1);
    }
  }
  oldA = newA;
  oldB = newB;
  return result;
}
