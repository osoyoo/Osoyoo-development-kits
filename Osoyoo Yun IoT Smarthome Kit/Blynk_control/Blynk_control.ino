#define BLYNK_PRINT Console
#include <Bridge.h>
#include <Console.h>
#include <BlynkSimpleYun.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";
void setup()
{
  Blynk.begin(auth);
  // Debug console
  Bridge.begin();
  Console.begin();
  while (!Console);{}
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8442);
}
void loop()
{
Blynk.run();
// You can inject your own code or combine it with other sketches.
// Check other examples on how to communicate with Blynk. Remember
// to avoid delay() function!
}
