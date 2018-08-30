
/*
 * This example demonstrate how to write data from your "Internet of Things" to IoTtweet dashboard
 * Compatible with "Espino32" by ThaiEasyElec.
 * Created : 2017.Aug.28
 * By Isaranu Janthong
 * IoTtweet Founder.
 * Visit us at : www.iottweet.com
 */

#include <IoTtweetESP32.h>

/* -- Espino32 : IoTtweet -- */
const char *userid = "userid";                  //IoTtweet account user ID (6 digits, included zero pre-fix)
const char *key = "device-key";                 //IoTtweet registered device key in "MY IOT Garage"
const char *ssid = "ssid";                      //Your-WiFi-router-SSID
const char *password = "password";              //Your-WiFi-password

float data0, data1, data2, data3;               //Your sending data variable.
String message_1 = "Hello from Espino32";       //Your private tweet meassage to dashboard
String message_2 = "ThaieasyElec na ja :)";     //Your public tweet message to dashboard

IoTtweetESP32 myiot;                            //naming your devices

void setup() {
  
  Serial.begin(115200);

  /* Get IoTtweet Library version */
  String libvers = myiot.getVersion();
  Serial.println("IoTtweet Library vesion : " + String(libvers));

  /* Connect WiFi */
  Serial.println("\nConnect wifi...");
  bool conn = myiot.begin(ssid,password);

  Serial.println("Lib.ver: " + String(libvers));
  if(!conn)
  {
      Serial.println("connect fails.");
    }else
    { 
      Serial.println("connected !.");
      }
}  

void loop() {

  /* - Example data generating -*/
  data0 = random(20,80);
  data1 = random(30,70);
  data2 = random(40,60);
  data3 = random(50,55);

  /*Send data from your iot to Dashboard */
  String response = myiot.WriteDashboard(userid,key,data0,data1,data2,data3,message_1,message_2);

  delay(15000);

}
