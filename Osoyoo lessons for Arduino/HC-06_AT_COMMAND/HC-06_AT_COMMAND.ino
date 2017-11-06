#include <SoftwareSerial.h>// import the serial library
#define ROBOT_NAME "HC-06"

#define BLUETOOTH_SPEED 9600 //This is the default baud rate that HC-06 uses

SoftwareSerial mySerial(10, 11); // TX | RX
// Connect the HC-06 TX to Arduino pin 10 RX.
// Connect the HC-06 RX to Arduino pin 11 TX.


void setup() {
   Serial.begin(9600);
 
  Serial.println("Starting the configurations!");
  mySerial.begin(BLUETOOTH_SPEED);
  delay(1000);

  // Should respond with OK
  Serial.print("AT test command is: ");
  mySerial.print("AT");
  waitResponse();

  Serial.println("----------------");

  // Should respond with its version
  Serial.print("AT version is: ");
  mySerial.print("AT+VERSION");
  waitResponse();

  Serial.println("----------------");
  // Set pin
  Serial.print("Set pin: ");
  mySerial.print("AT+PIN1234");
  waitResponse();

  Serial.println("----------------");
  // Set the name to ROBOT_NAME
  Serial.print("Set the name: ");
  String rnc = String("AT+NAME") + String(ROBOT_NAME);
  mySerial.print(rnc);
  waitResponse();

  //Set baudrate to 9600
  //AT+BAUD1  OK1200  Sets the baud rate to 1200
  //AT+BAUD2  OK2400  Sets the baud rate to 2400
  //AT+BAUD3  OK4800  Sets the baud rate to 4800
  //AT+BAUD4  OK9600  Sets the baud rate to 9600
  //AT+BAUD5  OK19200 Sets the baud rate to 19200
  //AT+BAUD6  OK38400 Sets the baud rate to 38400
  //AT+BAUD7  OK57600 Sets the baud rate to 57600
  //AT+BAUD8  OK115200  Sets the baud rate to 115200
  //AT+BAUD9  OK230400  Sets the baud rate to 230400
  //AT+BAUDA  OK460800  Sets the baud rate to 460800
  //AT+BAUDB  OK921600  Sets the baud rate to 921600
  //AT+BAUDC  OK1382400 Sets the baud rate to 1382400

  Serial.println("----------------");
  // Set baud rate to 9600
  Serial.print("Set baud rate: ");
  mySerial.print("AT+BAUD4");
  waitResponse();

  Serial.println("The configurations are done!");
}


void loop() {
 
  }

 
void waitResponse() {
    delay(2000);
    while (mySerial.available()) {
      Serial.write(mySerial.read());
    }
    Serial.write("\n");
}
