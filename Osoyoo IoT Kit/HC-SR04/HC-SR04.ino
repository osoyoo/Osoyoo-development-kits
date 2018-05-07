/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 2 
 Trig to Arduino pin 3
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "c4cb3ce0-4029-11e7-98bd-9ffdab1de3b9";
char password[] = "086740b8d0b9882b1191342fa0984c795cba7ca7";
char clientID[] = "a51c3620-4ea5-11e8-aaba-83a3e9c1fc99";

#define VIRTUAL_CHANNEL 11
#define echoPin 7
#define trigPin 8
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Cayenne.begin(username, password, clientID);
}

void loop()
{
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 //Delay 50ms before next reading.
 delay(50);  
 Serial.println(distance);
  Cayenne.loop();
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
  Cayenne.virtualWrite(VIRTUAL_CHANNEL, distance);
}
