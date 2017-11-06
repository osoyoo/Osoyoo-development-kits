/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *This example shows how you can fetch current time from 
 *the controller and put it to a battery backed up Real 
 *Time Clock (RTC) module attached to your Arduino. We've 
 *also attached a LCD Display showing time and temperature 
 *[coming from the internal temperature sensor in the RTC 
 *module].
 * Tutorial URL http://osoyoo.com/2017/09/12/arduino-lesson-real-time-clock-rtc-module-ds3231/
 */
#include <Wire.h>          // For the i2c devices
#include <LiquidCrystal_I2C.h>  // For the LCD
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define DS3231_I2C_ADDRESS 104    // RTC is connected, address is Hex68 (Decimal 104)

// SCL - pin A5
// SDA - pin A4
// To set the clock, run the sketch and use the serial monitor.
// Enter T1124154091014; the code will read this and set the clock. See the code for full details.
//
byte seconds, minutes, hours, day, date, month, year;
char weekDay[4];
byte tMSB, tLSB;
float my_temp;
char my_array[100];            // Character array for printing something.

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
}

void loop()
{
  watchConsole(); 
  get3231Date();
  Serial.print(weekDay); 
  Serial.print(", "); 
  Serial.print(date, DEC); 
  Serial.print("/"); 
  Serial.print(month, DEC);
  Serial.print("/"); 
  Serial.print(year, DEC); 
  Serial.print(" - ");
  Serial.print(hours, DEC);
  Serial.print(":"); 
  Serial.print(minutes, DEC);
  Serial.print(":"); 
  Serial.print(seconds, DEC);

  my_temp = (float)get3231Temp();
 
  Serial.print(" - Temp: "); 
  Serial.println(my_temp);
  
  // NOTE: Arduino does NOT implement printing floats to a string.
  // If you use the std C function : sprintf(my_array, "Temp: %4.2f", my_temp), It will NOT CONVERT.
  // So I abandoned this, since I don't need to print the float to the LCD anyway.
  
sprintf(my_array, "Time: %d:%d:%d", hours, minutes, seconds);

// Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(weekDay); 
  lcd.print(", ");
  lcd.print(date, DEC); 
  lcd.print("/"); 
  lcd.print(month, DEC);
  lcd.print("/"); 
  lcd.print(year, DEC);
  lcd.setCursor(0,1);
  lcd.print(my_array);
  delay(1000);
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

void watchConsole()
{
  if (Serial.available()) {      // Look for char in serial queue and process if found
    if (Serial.read() == 84) {      //If command = "T" Set Date
      set3231Date();
      get3231Date();
      Serial.println(" ");
    }
  }
}
 
void set3231Date()
{
//T(sec)(min)(hour)(dayOfWeek)(dayOfMonth)(month)(year)
//T(00-59)(00-59)(00-23)(1-7)(01-31)(01-12)(00-99)
//Example: 02-Feb-09 @ 19:57:11 for the 3rd day of the week -> T1157193020209
// T1124154091014
  seconds = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48)); // Use of (byte) type casting and ascii math to achieve result.  
  minutes = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  hours   = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  day     = (byte) (Serial.read() - 48);
  date    = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  month   = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  year    = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.write(decToBcd(seconds));
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(hours));
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(date));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}


void get3231Date()
{
  // send request to receive data starting at register 0
  Wire.beginTransmission(DS3231_I2C_ADDRESS); // 104 is DS3231 device address
  Wire.write(0x00); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // request seven bytes

  if(Wire.available()) {
    seconds = Wire.read(); // get seconds
    minutes = Wire.read(); // get minutes
    hours   = Wire.read();   // get hours
    day     = Wire.read();
    date    = Wire.read();
    month   = Wire.read(); //temp month
    year    = Wire.read();
       
    seconds = (((seconds & B11110000)>>4)*10 + (seconds & B00001111)); // convert BCD to decimal
    minutes = (((minutes & B11110000)>>4)*10 + (minutes & B00001111)); // convert BCD to decimal
    hours   = (((hours & B00110000)>>4)*10 + (hours & B00001111)); // convert BCD to decimal (assume 24 hour mode)
    day     = (day & B00000111); // 1-7
    date    = (((date & B00110000)>>4)*10 + (date & B00001111)); // 1-31
    month   = (((month & B00010000)>>4)*10 + (month & B00001111)); //msb7 is century overflow
    year    = (((year & B11110000)>>4)*10 + (year & B00001111));
  }
  else {
    //oh noes, no data!
  }
 
  switch (day) {
    case 1:
      strcpy(weekDay, "Sun");
      break;
    case 2:
      strcpy(weekDay, "Mon");
      break;
    case 3:
      strcpy(weekDay, "Tue");
      break;
    case 4:
      strcpy(weekDay, "Wed");
      break;
    case 5:
      strcpy(weekDay, "Thu");
      break;
    case 6:
      strcpy(weekDay, "Fri");
      break;
    case 7:
      strcpy(weekDay, "Sat");
      break;
  }
}

float get3231Temp()
{
  float temp3231;
  
  //temp registers (11h-12h) get updated automatically every 64s
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
 
  if(Wire.available()) {
    tMSB = Wire.read(); //2's complement int portion
    tLSB = Wire.read(); //fraction portion
   
    temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
    temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
  }
  else {
    //oh noes, no data!
  }
   
  return temp3231;
}
