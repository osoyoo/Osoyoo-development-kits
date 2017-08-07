const int NbrLEDs = 8;
const int ledPins[] = {5, 6, 7, 8, 9, 10, 11, 12};
const int photocellPin = A0;
int sensorValue = 0; // value read from the sensor
int ledLevel = 0; // sensor value converted into LED 'bars'
void setup() {
for (int led = 0; led < NbrLEDs; led++)
{
pinMode(ledPins[led], OUTPUT);// make all the LED pins outputs
}
}
void loop() {
sensorValue = analogRead(photocellPin);
ledLevel = map(sensorValue, 300, 1023, 0, NbrLEDs); // map to the number of LEDs
for (int led = 0; led < NbrLEDs; led++)
{
if (led < ledLevel ) {
digitalWrite(ledPins[led], HIGH); // turn on pins less than the level
}
else {
digitalWrite(ledPins[led],LOW); // turn off pins higher than
// the level
}
}
}
 
