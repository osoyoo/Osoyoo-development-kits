#define BLYNK_PRINT Console
#include "Console.h"
#include "Bridge.h"
#include "Console.h"
#include "BlynkSimpleYun.h"
#include "Servo.h"
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";

Servo myservo;

BLYNK_WRITE(V3)
{
  myservo.write(param.asInt());
}

void setup()
{
  Bridge.begin();
  Console.begin();
  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8442);

  myservo.attach(2);
  myservo.write(0);//back to 0 degrees 
  while (!Console);{}
}

void loop()
{
  Blynk.run();
}
