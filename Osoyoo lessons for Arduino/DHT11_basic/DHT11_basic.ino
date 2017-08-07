#include<dht.h>
dht DHT;
// if you require to change the pin number, Edit the pin with your arduino pin.
#define DHT11_PIN 3

void setup() 
{
Serial.begin(9600);
Serial.println("The real time Temperature and Humidity is :"); 
}

void loop() { // READ DATA
int chk = DHT.read11(DHT11_PIN);
Serial.print(" Humidity: " );
Serial.print(DHT.humidity, 1);
Serial.println('%');
Serial.print(" Temparature ");
Serial.print(DHT.temperature, 1);
Serial.println('C');
delay(2000);
}
