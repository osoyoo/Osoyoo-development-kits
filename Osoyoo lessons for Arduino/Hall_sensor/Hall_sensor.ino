const int ledPin = 13;//the led attach to pin13
int sensorPin = A0; // select the input pin for the potentiometer
int digitalPin=2; //D0 attach to pin2
int sensorValue = 0;// variable to store the value coming from A0
boolean digitalValue=0;// variable to store the value coming from pin2
void setup() 
{
  pinMode(digitalPin,INPUT);//set the state of D0 as INPUT
  pinMode(ledPin,OUTPUT);//set the state of pin13 as OUTPUT
  Serial.begin(9600); // initialize serial communications at 9600 bps
}
void loop() 
{ 
  sensorValue = analogRead(sensorPin); //read the value of A0
  digitalValue=digitalRead(digitalPin); //read the value of D0
  Serial.print("Sensor Value "); // print label to serial monitor 
  Serial.println(sensorValue); //print the value of A0
  Serial.print("Digital Value "); // print label to serial monitor 
  Serial.println(digitalValue); //print the value of D0 in the serial
  if( digitalValue==HIGH )//if the value of D0 is HIGH
  {
    digitalWrite(ledPin,LOW);//turn off the led
  }
  if( digitalValue==LOW)//else
  {
    digitalWrite(ledPin,HIGH);//turn on the led
  }
  delay(1000);//delay 200ms
}
