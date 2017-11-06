int switchPinFwd = 7;     //Input from the switch when in the Forward position
int potentiometerIn;      //variable to hold the potentiometer input

int fwdPin = 3;           //Logic level output to the H-Bridge (Forward)
int revPin = 4;           //Another logic level output to the H-Bridge (Reverse)

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPinFwd, INPUT_PULLUP);

  pinMode(fwdPin, OUTPUT); //Set the forward pin to an output
  pinMode(revPin, OUTPUT); //Set the forward pin to an output
}

void loop() {
  // put your main code here, to run repeatedly:
  potentiometerIn = analogRead(A0);
  int output = potentiometerIn / 4; //divide the potentiometer input by 4 so it can be used in the AnalogWrite function

  if(digitalRead(switchPinFwd) == HIGH) //Check to see if the pin is high or low
  {
    //If the pin is HIGH, it must be set to forward
      analogWrite(fwdPin, output); //Output our potentiometer value on the forward pin.
  }
  else
  {
    //Otherwise the switch must be set to Reverse
    analogWrite(revPin, output); //Output our potentiometer value on the forward pin.
  }
  delay(25);
}
