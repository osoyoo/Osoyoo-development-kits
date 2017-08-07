#include <IRremote.h>
const int irReceiverPin =2; //the SIG of receiver module attach to pin2 
const int ledPin = 3;//pin 3 connect to a led
IRrecv irrecv(irReceiverPin); //Creates a variable of type IRrecv
decode_results results;
void setup()
{
  pinMode(ledPin,OUTPUT);//set ledpin as OUTPUT
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
  if(results.value == 0xFFA25D)//if receiver module receive 0xFFA25D
  {
    digitalWrite(ledPin,HIGH);//turn on the led
  }
  else
  {
    digitalWrite(ledPin,LOW);//turn off the led
  }
}
