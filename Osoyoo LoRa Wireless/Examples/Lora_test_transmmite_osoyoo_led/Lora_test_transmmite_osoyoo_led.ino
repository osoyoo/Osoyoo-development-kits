#include <SoftwareSerial.h>

#define BTN1  4
#define BTN2  5  

SoftwareSerial loraSerial(2, 3); // TX, RX

String turnOn = "on";
String turnOff = "off";


void setup() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  Serial.begin(9600);
  loraSerial.begin(9600);
  
}

void loop() {

  if(digitalRead(BTN1) == 0) {
    loraSerial.print(turnOn);
    while(digitalRead(BTN1) == 0);
    delay(50);
  }

  if(digitalRead(BTN2) == 0) {
    loraSerial.print(turnOff);
    while(digitalRead(BTN2) == 0);
    delay(50);
  }
}
