#define LED 13 // Set D13 as the LED pin
#define BUTTON 12 //Set D12 as the button pin
//Let's say you have your push button on pin 12
int switchState = 0; // actual read value from pin12
int oldSwitchState = 0; // last read value from pin12
int lightsOn = 0; // is the switch on = 1 or off = 0

void setup() { 
 pinMode(BUTTON, INPUT); // set the push button as input
 pinMode(LED, OUTPUT); // anything you want to control using a switch e.g. a Led
}
void loop() {
 switchState = digitalRead(BUTTON); // read the pushButton State
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
 digitalWrite(LED, HIGH); // set the LED on
 } else {
 digitalWrite(LED, LOW); // set the LED off
 }
 }

