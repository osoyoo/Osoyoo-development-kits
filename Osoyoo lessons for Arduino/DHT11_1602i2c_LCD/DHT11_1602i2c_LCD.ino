#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<dht.h>
dht DHT;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define DHT11_PIN 3

void setup()
{
 lcd.begin(16,2);
 lcd.init();                      // initialize the lcd 
 // Print a message to the LCD.
 lcd.backlight();
 lcd.clear();
 lcd.print("Humidity & temp");
 delay(3000);
 lcd.clear();
 lcd.print("Starting.....");
 delay(3000);
}

void loop() { // READ DATA
 int chk = DHT.read11(DHT11_PIN);
 lcd.clear();
 delay(500);
 lcd.setCursor(0, 0);
 // print from 0 to 9:
 lcd.print("Temp    : ");
 lcd.print(DHT.temperature, 1);
 lcd.print(" C");  
 // set the cursor to (16,1):
 lcd.setCursor(0,1);
 lcd.print("Humidity: ");
 lcd.print(DHT.humidity, 1);
 lcd.print(" %");
 delay(2000);
}
