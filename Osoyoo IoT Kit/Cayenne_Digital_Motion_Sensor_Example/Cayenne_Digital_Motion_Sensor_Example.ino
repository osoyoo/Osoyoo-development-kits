/*
Cayenne Digital Motion Sensor Example

This sketch shows how to automatically send data to a Digital Motion Sensor in the Cayenne Dashboard.

The Cayenne Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. In the Cayenne Dashboard add a new Digital Motion Sensor widget.
2. Set the widget to 2-State Display.
3. Select Virtual Pins and a virtual pin number.
4. Set VIRTUAL_PIN to the pin number you selected.
5. Attach a Digital Motion Sensor to a digital pin on your Arduino.
   Schematic:
   [Ground] -- [Digital Motion Sensor] -- [5V]
                         |
                     Digital Pin
6. Set the motionSensorPin variable to match the pin used to connect the motion sensor.
7. Set the token variable to match the Arduino token from the Dashboard.
8. Compile and upload this sketch.
9. Once the Arduino connects to the Dashboard it should automatically update the Digital Motion Sensor widget with data.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space

// If you're not using the Ethernet W5100 shield, change this to match your connection type. See Communications examples.
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "c4cb3ce0-4029-11e7-98bd-9ffdab1de3b9";
char password[] = "086740b8d0b9882b1191342fa0984c795cba7ca7";
char clientID[] = "a51c3620-4ea5-11e8-aaba-83a3e9c1fc99";

// Virtual Pin of the Digital Motion Sensor widget.
#define VIRTUAL_CHANNEL 14

// Digital pin the motion sensor is connected to. Do not use digital pins 0 or 1 since those conflict with the use of Serial.
const int motionSensorPin = 3;
const int buzzerPin = 4;
void setup()
{
  Serial.begin(9600);
  pinMode(motionSensorPin,INPUT);
  pinMode(buzzerPin,OUTPUT);
  digitalWrite(buzzerPin,HIGH);
  Cayenne.begin(username, password, clientID);
}

void loop()
{
  Cayenne.loop();
  checkSensor();
}

int currentState = -1;
unsigned long previousMillis = 0;

void checkSensor()
{
  unsigned long currentMillis = millis();
  // Check sensor data every 250 milliseconds
  if (currentMillis - previousMillis >= 250) {
    // Check the sensor state and send data when it changes.
    currentState = digitalRead(motionSensorPin);
    Cayenne.virtualWrite(VIRTUAL_CHANNEL, currentState);
    if (currentState == HIGH) {
      digitalWrite(buzzerPin,LOW);
    }
    else {
      digitalWrite(buzzerPin,HIGH);
    }
        previousMillis = currentMillis;
  }
}
