#define BLYNK_PRINT Console
#include <Bridge.h>
#include <Console.h>
#include <BlynkSimpleYun.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";
SimpleTimer timer;

//Widget LED
WidgetLED led1(V0); //register to virtual pin 0

//smoke sensor mq2
//connect to A1
 int smoke_mq2 = A1;
int smoke = 0; 
//int Buzzer = 31;
void SMOKESENSOR () {
  Console.println("SMOKE SENSOR START!");
  smoke = analogRead(smoke_mq2);
  Console.print("smoke_val = ");
  Console.println(smoke);   //The serial will print the smoke value
  if(smoke>=130)       // the point at which the state of LEDs change 
  {
    //digitalWrite(Buzzer, HIGH);
    led1.on();
  }
  else
  {
    Console.println("No smoke!");
    //digitalWrite(Buzzer, LOW);
    led1.off();
  } 
  //  delay(500);
}

void setup()
{
  Blynk.begin(auth); 
  Bridge.begin();
  Console.begin();
  timer.setInterval(10000, SMOKESENSOR);
  while (!Console);{}
    
}

void loop() {
  // this is where the "polling" occurs
  timer.run();
  Blynk.run();

}
