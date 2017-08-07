/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * In this lesson, we will monitor the output of a photoresistor, allow 
 * the Arduino to know how light or dark it is. When the light falls below 
 * a certain level, the Arduino turns on an LED.
 * Tutorial URL:http://osoyoo.com/2017/07/05/arduino-lesson-photoresistor/
 * CopyRight www.osoyoo.com
 */
int photocellPin = A0;    // select the input pin for the photoresistor
int ledPin = 9;   // select the pin for the LED 
int val = 0;       // variable to store the value coming from the sensor 

void setup() { 
  Serial.begin(9600);//Set the baudrate to 9600,make sure it's same as your software settings
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT  
   pinMode(photocellPin, INPUT);  // declare the ledPin as an OUTPUT  
} 

void loop() { 
  val = analogRead(photocellPin);   // read the value from the sensor  
  Serial.println(val);   //The serial will print the light value
  if(val<=512)       // the point at which the state of LEDs change 
  {
    digitalWrite(ledPin, HIGH); // set LED on
  }
  else
  {
    digitalWrite(ledPin, LOW);  //set LED off
  }
} 
