/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *In this lesson, we will show how to use program to make LED brighter and darker, just 
 *like breathing,so we called it breathing light here.
 *
 * Tutorial URL http://osoyoo.com/2017/06/30/arduino-lesson-pwm-control-led-brightness/
 * CopyRight www.osoyoo.com
 */
void setup ()
{
  pinMode(3,OUTPUT);// declare pin 3 to be an output
}

void loop()
{
  for (int a=0; a<=255;a++)     //loop from 0 to 255,it controls the increase in PWM brightness
  {
    analogWrite(3,a);           //set the brightness of pin 3:          
    delay(8);                   //wait for 8 ms        
  }
    for (int a=255; a>=0;a--)   //loop from 255 down to 0,it control PWM brightness reduction
  {
    analogWrite(3,a);           // set the brightness of pin 3:
    delay(8);                   //wait for 8 ms
  }
  delay(800);                  //wait for 800 ms
}
