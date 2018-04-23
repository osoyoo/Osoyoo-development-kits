#include <Bridge.h>
#include <Console.h>
#include <SimpleTimer.h>
#include <SPI.h>
SimpleTimer timer;
#define echoPin 29
#define trigPin 27
int lightsensorPin = A0;    // select the input pin for the photoresistor
//int ledPin = 9;   // select the pin for the LED 
int light_val = 0;       // variable to store the value coming from the sensor 
int LED = 26;   // select the pin26 for the LED 

//Light / Ultrasonic

void LIGHTSENSOR () {
    Console.println("LIGHT SENSOR AND HC-SR04 START!");
  float duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  
light_val = analogRead(lightsensorPin);   // read the value from the sensor  
    Console.print("light_val = ");
    Console.println(light_val);   //The serial will print the light value
    Console.print("Distance = ");
    Console.print(distance);
    Console.println(" cm");
  if(light_val>=400 && distance <= 10  )       // the point at which the state of LEDs change 
  {
    digitalWrite(LED, HIGH);
    delay(1000);
  }
  else
  {
     digitalWrite(LED, LOW);  //set LED off
  } 
}

void setup()
{
 // Blynk.begin(auth); 
  Bridge.begin();
  Console.begin();
  SPI.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);      // declare LED as output
  timer.setInterval(1000, LIGHTSENSOR);
  while (!Console);{}
}
void loop() {
  timer.run();
}

