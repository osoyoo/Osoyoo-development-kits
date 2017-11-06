/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *In this example you will use three potentiometers to control 
 *each pin of the RGB LED to produce any color you want.
 * Tutorial URL http://osoyoo.com/2017/08/23/arduino-lesson-rgb-led/
 * CopyRight www.osoyoo.com
 */
const int redPin = 11; // R connected to digital pin 11 
const int greenPin = 9; // G to digital pin 10 
const int bluePin = 9; // B connected to digital pin 9 

void setup()
{ 
pinMode(redPin, OUTPUT); // set the redPin to be an output 
pinMode(greenPin, OUTPUT); // set the greenPin to be an output 
pinMode(bluePin, OUTPUT); // set the bluePin to be an output 
} 

void loop() // run over and over again 
{ 
// Basic colors: 
color(255, 0, 0); // turn the RGB LED red 
delay(1000); 
color(0,255, 0); // turn the RGB LED green 
delay(1000); 
color(0, 0, 255); // turn the RGB LED blue 
delay(1000); 
// Example blended colors: 
color(255,0,252); // turn the RGB LED red 
delay(1000); 
color(237,109,0); // turn the RGB LED orange 
delay(1000);
color(255,215,0); // turn the RGB LED yellow 
delay(1000); 
color(34,139,34); // turn the RGB LED green 
delay(1000); 
color(0,112,255); // turn the RGB LED blue 
delay(1000); 
color(0,46,90); // turn the RGB LED indigo 
delay(1000); 
color(128,0,128); // turn the RGB LED purple 
delay(1000); 
} 
/******************************************************/
void color (unsigned char red, unsigned char green, unsigned char blue) // the color generating function 
{ 
analogWrite(redPin, red); 
analogWrite(greenPin, green); 
analogWrite(bluePin, blue); 
}
