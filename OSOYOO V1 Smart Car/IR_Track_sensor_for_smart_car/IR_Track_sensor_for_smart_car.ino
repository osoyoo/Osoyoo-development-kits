/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * This is the Arduino Hello Worls sketch.
 *In this experiment, we will use an IR track sensor module 
 *and the on-board LED to build a simple circuit to make 
 *a tracking line. Since the LED has been attached to 
 *pin 13, connect the pin D0 to digital pin 2 of the Uno 
 *board. When the tracking sensor detects reflection 
 *signals (white), the LED will be on. Otherwise, 
 *it will be off (black line).
 * CopyRight www.osoyoo.com
 */
const int trackingPin = 2; //the tracking module attach to pin 2
const int ledPin = 13; //pin13 built-in led
void setup()
{
  Serial.begin(9600);
  pinMode(trackingPin, INPUT); // set trackingPin as INPUT
  pinMode(ledPin, OUTPUT); //set ledPin as OUTPUT
}
void loop()
{
  boolean val = digitalRead(trackingPin); // read the value of tracking module
  if(val == HIGH) //if it is HiGH
  { 
    digitalWrite(ledPin, LOW); //turn off the led
    Serial.println("Detect: Black!");
  }
  else
  {
    digitalWrite(ledPin, HIGH); //turn on the led
    Serial.println("Detect: White!");
  }
}
