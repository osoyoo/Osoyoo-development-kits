#include <IRremote.h>
const int irReceiverPin =3; //the SIG of receiver module attach to pin3
const int buzzerPin = 13;//pin 13 connect to a buzzer
IRrecv irrecv(irReceiverPin); //Creates a variable of type IRrecv
decode_results results;
void setup()
{
  pinMode(buzzerPin,OUTPUT);//set buzzer pin as OUTPUT
  digitalWrite(buzzerPin,HIGH);
  Serial.begin(9600);//initialize serial 
  irrecv.enableIRIn(); //enable ir receiver module 
}
void loop() 
{
  if (irrecv.decode(&results)) //if the ir receiver module receiver data
  { 
    Serial.print("irCode: "); //print"irCode: " 
    Serial.print(results.value, HEX); //print the value in hexdecimal 
    Serial.print(", bits: "); //print" , bits: " 
    Serial.println(results.bits); //print the bits
    irrecv.resume(); // Receive the next value 
  } 
  delay(600); //delay 600ms
  if(results.value == 0xFF38C7)//if receiver module receive 0xFF38C7
  {
    digitalWrite(buzzerPin,LOW);//buzzer beep
  }
  else
  {
    digitalWrite(buzzerPin,HIGH);//stop beep
  }
}
