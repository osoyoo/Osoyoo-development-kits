int pin3=3; //Connect the matrix pin 9 to Uno pin 3
int pin11=11;//Connect the matrix pin 13 to Uno pin 11
void setup() {
// put your setup code here, to run once:
pinMode(pin3,OUTPUT);
pinMode(pin11,OUTPUT);
digitalWrite(pin3,HIGH);
digitalWrite(pin11,HIGH);
}
void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(pin11,LOW);//set the pin11 low，led will be turned on 
delay(200);
digitalWrite(pin11,HIGH);//set the pin 11 high，led will be turned off。
delay(200);
}
