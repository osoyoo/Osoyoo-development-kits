/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *This is a example to blink an LED
 *In this part we will use different resistor to change the brightness of an LED.
 * Tutorial URL http://osoyoo.com/2017/06/28/arduino-lesson-led/
 * CopyRight www.osoyoo.com
 */
// We use the pin 2 to connect with the LED;.
// give it a name:
int led = 2;
 
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}
 
// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for 0.5 seconds
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for 0.5 seconds
}
