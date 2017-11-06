/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *In this experiment, connect each of pin A-G of the 7-Segment Display 
 *to one 200 ohm current limiting resistor respectively and then to 
 *pin 4–11. GND connects to GND. By programming, we can set one or 
 *several of pin4-11 as High level to light up the corresponding LED(s).
 * Tutorial URL http://osoyoo.com/2017/07/18/arduino-lesson-one-digit-7-segment-led-display/
 * CopyRight www.osoyoo.com
 */
int a=7; 
int b=6; 
int c=5; 
int d=11; 
int e=10; 
int f=8; 
int g=9; 
int dp=4; 
//display number 1
void display1(void) 
{ 
    digitalWrite(b,HIGH);
    digitalWrite(c,HIGH);
} 
//display number2
void display2(void) 
{
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(g,HIGH);
  digitalWrite(e,HIGH);
    digitalWrite(d,HIGH);
} 
// display number3
void display3(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    
  digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);
    digitalWrite(g,HIGH);
} 
// display number4
void display4(void) 
{ 
    digitalWrite(f,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(g,HIGH);
  digitalWrite(c,HIGH);
  
} 
// display number5
void display5(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);
  digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);
} 
// display number6
void display6(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);
  digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);  
    digitalWrite(e,HIGH);  
} 
// display number7
void display7(void) 
{   
   digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(c,HIGH);
} 
// display number8
void display8(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(g,HIGH);
  digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);  
    digitalWrite(e,HIGH);  
  digitalWrite(f,HIGH);  
} 
void clearDisplay(void) 
{ 
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(g,LOW);
  digitalWrite(c,LOW);
    digitalWrite(d,LOW);  
    digitalWrite(e,LOW);  
  digitalWrite(f,LOW);  
} 
void display9(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(g,HIGH);
  digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);  
  digitalWrite(f,HIGH);  
} 
void display0(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);  
    digitalWrite(e,HIGH);  
  digitalWrite(f,HIGH);  
} 
void setup() 
{ 
    int i;
    for(i=4;i<=11;i++) 
        pinMode(i,OUTPUT);
} 
void loop() 
{     
    while(1) 
    {   clearDisplay();
  display0(); 
        delay(2000); 
        clearDisplay();
        display1(); 
        delay(2000); 
        clearDisplay();
        display2(); 
        delay(2000); 
        clearDisplay();
        display3(); 
        delay(2000);
        clearDisplay();
        display4(); 
        delay(2000);
        clearDisplay(); 
        display5(); 
        delay(2000);
        clearDisplay(); 
        display6(); 
        delay(2000);
        clearDisplay(); 
        display7(); 
        delay(2000); 
        clearDisplay();
        display8(); 
        delay(2000); 
        clearDisplay();  
        display9(); 
        delay(2000);      
    }
}


