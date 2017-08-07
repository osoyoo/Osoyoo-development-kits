const int sigPin = 2; // the number of the tilt switch pin
const int ledPin = 13; // the number of the LED pin
// variables will change:
boolean sigState = 0; // variable for reading the tilt switch status
void setup()
{
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT); 
  // initialize the tilt switch pin as an input:
  pinMode(sigPin, INPUT); 
}
void loop()
{
  // read the state of the tilt switch value:
  sigState = digitalRead(sigPin);
  if (sigState == HIGH)
  { 
    // turn LED on: 
    digitalWrite(ledPin, LOW); 
  } 
  else
  {
    // turn LED off:
    digitalWrite(ledPin, HIGH); 
  }
}
