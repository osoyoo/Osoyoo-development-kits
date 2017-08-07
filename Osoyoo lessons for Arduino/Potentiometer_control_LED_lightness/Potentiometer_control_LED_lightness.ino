/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * Controlling led by potentiometer
 * Rotate the shaft of the potentiometer and you should see the luminance of the LED change.
 * Tutorial URL http://osoyoo.com/2017/07/04/arduino-lesson-potentiometer/
 * CopyRight www.osoyoo.com
 */
const int analogPin = A0;//the analog input pin attach to
const int ledPin = 9;//the led attach to
int inputValue = 0;//variable to store the value coming from sensor
int outputValue = 0;//variable to store the output value

void setup()
{
}

void loop()
{
inputValue = analogRead(analogPin);//read the value from the sensor
outputValue = map(inputValue,0,1023,0,255);//Convert from 0-1023 proportional to the number of a number of from 0 to 255
analogWrite(ledPin,outputValue);//turn the led on depend on the output value
}
