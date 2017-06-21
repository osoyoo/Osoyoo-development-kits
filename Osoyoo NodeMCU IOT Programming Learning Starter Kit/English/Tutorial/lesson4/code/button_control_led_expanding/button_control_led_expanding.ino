//Let's say you have your push button on pin 2
int switchState = 0; // actual read value from pin2
int oldSwitchState = 0; // last read value from pin2

int lightsOn = 0; // is the switch on = 1 or off = 0

void setup() {                
  pinMode(4, INPUT); // push button     
  pinMode(5, OUTPUT); // anything you want to control using a switch e.g. a Led
}

void loop() {
  switchState = digitalRead(4); // read the pushButton State
  if (switchState != oldSwitchState) // catch change
  {
    oldSwitchState = switchState;
    if (switchState == HIGH)
    {
      // toggle
      lightsOn = !lightsOn;
    }
  }

  if(lightsOn)
  {
     digitalWrite(5, HIGH);   // set the LED on
   } else {
      digitalWrite(5, LOW);   // set the LED off
    }
  }

