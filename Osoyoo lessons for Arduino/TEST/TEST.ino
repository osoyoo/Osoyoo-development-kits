#include <dht.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
dht DHT;//create a variable type of dht
const int DHT11_PIN= 7;//Humiture sensor attach to pin7
void setup()
{
  Serial.begin(9600);//initialize the serial
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
}
void loop()
{
  // READ DATA
  //Serial.println("DHT11:");

  int chk = DHT.read11(DHT11_PIN);//read the value returned from sensor
  switch (chk)
  {
    case DHTLIB_OK: 
    //Serial.println("OK!"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    //Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    //Serial.print("Time out error,\t"); 
    break;
    default: 
    //Serial.print("Unknown error,\t"); 
    break;
  }
  // DISPLAY DATA
  lcd.setCursor(0, 0);
  lcd.print("Tem:");
  //Serial.print("Tem:");
  lcd.print(DHT.temperature,1); //print the temperature on lcd
  // Serial.print(DHT.temperature,1);
  lcd.print(char(223));//print the unit" ℃ "
  lcd.print("C");
  // Serial.println(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  //Serial.print("Hum:");
  lcd.print(DHT.humidity,1); //print the humidity on lcd
  //Serial.print(DHT.humidity,1);
  lcd.print(" %"); 
  //Serial.println(" %");
  delay(200); //wait a while 
}

