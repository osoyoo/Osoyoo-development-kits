#define BLYNK_PRINT Console
#include <Bridge.h>
#include <Console.h>
#include <BlynkSimpleYun.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";
SimpleTimer timer;

//Widget LED
WidgetLED led4(V4); //register to virtual pin 4

//FLAME sensor
//connect to A2
int FlamePin = A2;  // This is for input pin
int Flame = 0;  // HIGH when FLAME Exposed

void FLAMESENSOR () {
  Console.println("FLAME SENSOR START!");
  Flame = analogRead(FlamePin);
  Console.print("flame_val = ");
  Console.println(Flame);   //The serial will print the smoke value
  if(Flame<=700)       // the point at which the state of LEDs change 
  {
    led4.on();
  }
  else
  {
    Console.println("No Fire!");
    led4.off();
  } 
    //delay(2000);
}

void setup()
{
  Blynk.begin(auth); 
  Bridge.begin();
  Console.begin();
  timer.setInterval(10000, FLAMESENSOR);
  while (!Console);{}
    
}

void loop() {
  // this is where the "polling" occurs
  timer.run();
  Blynk.run();

}
