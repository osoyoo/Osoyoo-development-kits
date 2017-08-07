/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * This is the Arduino Eight of Flowing Water Light Example
 * In this lesson we will realize the function of eight of flowing water light, 
 * from left to right, each only light one LED,reciprocating cycle.
 * Tutorial URL http://osoyoo.com/2017/06/29/arduino-lesson-eight-of-flowing-water-light/
 * CopyRight www.osoyoo.com
 */
void setup()
{
  for (int i=2; i<=9; i++)    //Set the pin2~pin8 cycle output
  {
    pinMode(i,OUTPUT);
  }
}
void loop()
{
  for (int x=2; x<=9; x++)   //Each LED will flash in one second
  {
    digitalWrite(x,HIGH);
    delay(500);
    digitalWrite(x,LOW);
    delay(500);
  }
}
