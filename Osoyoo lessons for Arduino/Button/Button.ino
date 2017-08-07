/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * In this lesson, we will show how to turn on/off an LED by 
 * using an I/O port and a button.
 * Tutorial URLhttp://osoyoo.com/2017/07/02/arduino-lesson-button/
 * CopyRight www.osoyoo.com
 */
const int keyPin = 12; //the number of the key pin
const int ledPin = 13;//the number of the led pin

void setup()
{
pinMode(keyPin,INPUT);//initialize the key pin as input 
pinMode(ledPin,OUTPUT);//initialize the led pin as output
}

void loop()
{
//read the state of the key value
//and check if the kye is pressed
//if it is,the state is HIGH 
if(digitalRead(keyPin) ==HIGH )
{
digitalWrite(ledPin,HIGH);//press the button to turn on the led
}
else
{
digitalWrite(ledPin,LOW);//release the button to turn off the led
}
}
