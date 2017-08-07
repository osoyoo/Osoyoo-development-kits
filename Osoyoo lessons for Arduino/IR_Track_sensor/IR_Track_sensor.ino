const int trackingPin = 2; //the tracking module attach to pin 2
const int ledPin = 13; //pin13 built-in led
void setup()
{
  pinMode(trackingPin, INPUT); // set trackingPin as INPUT
  pinMode(ledPin, OUTPUT); //set ledPin as OUTPUT
}
void loop()
{
  boolean val = digitalRead(trackingPin); // read the value of tracking module
  if(val == HIGH) //if it is HiGH
  { 
    digitalWrite(ledPin, LOW); //turn off the led
  }
  else
  {
    digitalWrite(ledPin, HIGH); //turn on the led
  }
}
