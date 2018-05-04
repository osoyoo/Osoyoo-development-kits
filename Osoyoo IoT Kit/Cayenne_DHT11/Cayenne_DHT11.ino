#include "DHT.h"
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "c4cb3ce0-4029-11e7-98bd-9ffdab1de3b9";
char password[] = "086740b8d0b9882b1191342fa0984c795cba7ca7";
char clientID[] = "a51c3620-4ea5-11e8-aaba-83a3e9c1fc99";

#define DHTPIN 5 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.
#define VIRTUAL_CHANNEL1 3
#define VIRTUAL_CHANNEL2 4
// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);


void setup()
{
  Serial.begin(9600);
  dht.begin();
  Cayenne.begin(username, password, clientID);
}

void loop()
{
   float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  Serial.print("The temperature is :");
  Serial.println(t);
  Serial.print("The humidity is :");
  Serial.println(h);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }
  Cayenne.loop();
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL1)
{
  Cayenne.virtualWrite(VIRTUAL_CHANNEL1, dht.readTemperature());
}
CAYENNE_OUT(VIRTUAL_CHANNEL2)
{
  Cayenne.virtualWrite(VIRTUAL_CHANNEL2, dht.readHumidity());
}
