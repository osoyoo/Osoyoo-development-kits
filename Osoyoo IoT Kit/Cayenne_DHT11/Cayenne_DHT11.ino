#include "DHT.h"
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "put your username here ";
char password[] = "put your password here";
char clientID[] = "put your clientID here";

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
