
#include <NewSoftSerial.h>

NewSoftSerial mySerial =  NewSoftSerial(2, 3);

void setup()  {
  pinMode(13, OUTPUT);
  Serial.begin(57600);
  Serial.println("Goodnight moon!");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
}

void loop()                     // run over and over again
{

  if (mySerial.available()) {
      Serial.print((char)mySerial.read());
  }
  if (Serial.available()) {
      mySerial.print((char)Serial.read());
  }
}
