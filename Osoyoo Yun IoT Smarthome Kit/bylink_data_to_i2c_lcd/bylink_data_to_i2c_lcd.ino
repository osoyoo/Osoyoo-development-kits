
/* *************************************************************

  You can use this sketch as a debug tool that prints all incoming values
  sent by a widget connected to a Virtual Pin 1 in the Blynk App.

  App project setup:
    Slider widget (0...100) on V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Bridge.h>
#include <BlynkSimpleYun.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";
LiquidCrystal_I2C lcd(0x27,16,2);
// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V10)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
 lcd.setCursor(3,0);
  lcd.print("V1 Slider value is: ");
    lcd.setCursor(2,1);
  lcd.println(pinValue);
}

void setup()
{
  // Debug console
  //Serial.begin(9600);
  Bridge.begin();
 lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8442);
}

void loop()
{
  Blynk.run();
}

