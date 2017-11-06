int pin = 7; //pin 7 connected to the third pin of NE555
unsigned long duration; //the variable to store the length of the pulse 

void setup()
{
pinMode(pin, INPUT); //set the pin as an input
Serial.begin(9600); // start serial port at 9600 bps
}

void loop()
{
duration = pulseIn(pin, HIGH); //Reads a pulse on pin
Serial.print(duration); //print the length of the pulse on the serial monitor
Serial.println(); //print an blank on serial monitor
delay(500); //wait for 500 ms
}
