int ledPin = 13;                // choose the pin for the LED
int relayInput = 3;             // choose the pin for the relay
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
 
void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(relayInput, OUTPUT);  // declare relay as output
  digitalWrite(relayInput, HIGH);//assuming relay is off
  Serial.begin(9600);
}
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      digitalWrite(relayInput, LOW); // The Relay Input works Inversly
      Serial.println("Turn on the Lamp!");
      Serial.println("Wait for 30 seconds");
      delay(30000);              // delay for 30 seconds
      digitalWrite(relayInput, HIGH);// Relay input operation is positive
      Serial.println("Turn off the Lamp!");
    }
  } 
  else 
  {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH)
    {
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}
