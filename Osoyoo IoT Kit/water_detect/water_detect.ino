/*
Cayenne Generic Analog Input Example

This sketch shows how to send Generic Analog Input Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach an analog input device (e.g. a potentiometer) to an analog pin on your Arduino. Make sure to use an analog pin, not a digital pin.
2. Set the SENSOR_PIN value below to the pin number you used when connecting the sensor.
3. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of an Generic Analog Input Sensor widget you have added) in the Dashboard.
4. Set the Cayenne authentication info to match the authentication info from the Dashboard.
5. Compile and upload this sketch.
6. Once the Arduino connects to the Dashboard it should automatically create a temporary display widget (or update the Generic Analog Input Sensor widget you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "c4cb3ce0-4029-11e7-98bd-9ffdab1de3b9";
char password[] = "086740b8d0b9882b1191342fa0984c795cba7ca7";
char clientID[] = "a51c3620-4ea5-11e8-aaba-83a3e9c1fc99";

#define SENSOR_PIN 0
#define VIRTUAL_CHANNEL 8
int sensorValue = 0;
void setup()
{
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID);
}

void loop()
{
 sensorValue = analogRead(SENSOR_PIN); 
 Serial.print("Sensor = " ); 
 Serial.print(sensorValue*100/1024); 
 Serial.println("%");
 delay(1000); 
  Cayenne.loop();
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
   sensorValue = analogRead(SENSOR_PIN); 
  Cayenne.virtualWrite(VIRTUAL_CHANNEL, analogRead(SENSOR_PIN)*100/1024);
}
