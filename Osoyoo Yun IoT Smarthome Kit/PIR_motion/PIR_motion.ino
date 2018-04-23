#define BLYNK_PRINT Console
#include <Bridge.h>
#include <Console.h>
#include <BlynkSimpleYun.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";
SimpleTimer timer;
int Buzzer = 31;//the pin of the active buzzer
//PIR_HC_SR501
int pir_hc_sr501 = 24;          // choose the pin for the PIR
int pirState = LOW;             // we start, assuming no motion detected
int pirval = 0;                    // variable for reading the pin status
WidgetLED led2(V1); //register to virtual pin 1

//PIR HC-SR501
void PIRSENSOR () {
    Console.println("PIR SENSOR START!");
    pirval = digitalRead(pir_hc_sr501);  // read input value
    if (pirval == HIGH) {            // check if the input is HIGH
    led2.on(); // set LED on
    digitalWrite(Buzzer, HIGH);
    if (pirState == LOW) {
    // we have just turned on
    Console.println("Motion detected!");
    // We only want to print on the output change, not state
    pirState = HIGH;
    }
  } 
  else 
  {
    led2.off();  //set LED off
    if (pirState == HIGH){
    // we have just turned of
    digitalWrite(Buzzer, LOW);
    Console.println("Motion ended!");
    // We only want to print on the output change, not state
    pirState = LOW;
    }
  }
}

void setup()
{
  Blynk.begin(auth); 
  Bridge.begin();
  Console.begin();
  pinMode(Buzzer, OUTPUT);      // declare Buzzer as output
  pinMode(pir_hc_sr501, INPUT);     // declare sensor as input
  timer.setInterval(1000, PIRSENSOR);
  while (!Console);{}
}
void loop() {
  // this is where the "polling" occurs
  timer.run();
  Blynk.run();
}

