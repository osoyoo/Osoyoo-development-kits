/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * This sketch is about how to use the MQTT protocol to control a step motor
 * Tutorial URL  http://osoyoo.com/2017/05/17/nodemcu-lesson-16-step-motor-mqtt/
 * CopyRight www.osoyoo.com
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>
// Update these with values suitable for your network.
const char* ssid = "*****";//put your wifi ssid here
const char* password = "*******";//put your wifi password here
 const char* mqtt_server = "broker.mqttdashboard.com";
//const char* mqtt_server = "iot.eclipse.org";

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// initialize the stepper library on D1,D2,D5,D6
Stepper myStepper(stepsPerRevolution, D1, D2, D5, D6);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  int p =(char)payload[0]-'0';
  // step one revolution  in one direction:
  if(p==1) 
  {
    myStepper.step(stepsPerRevolution);
    Serial.print("  clockwise" );
   }
  // step one revolution in the other direction:
  else if(p==2)
  {
    myStepper.step(-stepsPerRevolution);
    Serial.print("  counterclockwise" );
   }
   Serial.println();
}
 
//  Serial.println();
 //end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("OsoyooCommand");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // set the speed at 80 rpm:
  myStepper.setSpeed(80);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
