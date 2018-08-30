
/*
 * This example demonstrate how to write data from your "Internet of Things" to IoTtweet dashboard
 * Compatible with "Espino32" by ThaiEasyElec.
 * Created : 2017.Aug.28
 * By Isaranu Janthong
 * IoTtweet Founder.
 * Visit us at : www.iottweet.com
 */

#include "DHT.h"

/* -- Espino32 : DHT11 -- */
#define DHTPIN 10
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float h,t,f,hif,hic;
float t_offset = 0.0, h_offset = 0.0;

void setup() {
  
  Serial.begin(115200);
  dht.begin();
  
}  

void loop() {
  
  /* - DHT sensor reading - */
  float h = dht.readHumidity() + h_offset;
  float t = dht.readTemperature() + t_offset;
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Temperature is " + String(t) + " celcuis");
  Serial.println("Humidity is " + String(h) + " %RH");
  Serial.println("--------------------");
 
  delay(2000);

}
