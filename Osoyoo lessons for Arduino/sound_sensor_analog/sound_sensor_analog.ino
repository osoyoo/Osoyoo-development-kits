const int ledPin = 13; //pin 13 built-in led
const int soundPin = A0; //sound sensor attach to A0
int threshold = 600; //Set minimum threshold for LED lit
void setup()
{
  pinMode(ledPin,OUTPUT);//set pin13 as OUTPUT
  Serial.begin(9600); //initialize serial
}
void loop()
{
  int value = analogRead(soundPin);//read the value of A0
  Serial.println(value);//print the value
  if(value > threshold) //if the value is greater than 600
  {
    digitalWrite(ledPin,HIGH);//turn on the led
    delay(200);//delay 200ms
  }
  else
  {
    digitalWrite(ledPin,LOW);//turn off the led
  }
  delay(1000);
}

