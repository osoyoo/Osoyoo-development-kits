int digit_sensor = 2; // select the input pin for the potentiometer
int ledPin = 13; // select the pin for the LED
int digitValue ;  //  value from the digit input pin

void setup () {
  pinMode (ledPin, OUTPUT);
  pinMode (digit_sensor, INPUT);
  Serial.begin (9600);
}

void loop () {
  digitValue=digitalRead(digit_sensor);
  if (digitValue==LOW)
  {
    digitalWrite (ledPin, HIGH);
    delay(50);
    }   
  else 
  {
    digitalWrite (ledPin, LOW);
    delay(10);
    }
}
