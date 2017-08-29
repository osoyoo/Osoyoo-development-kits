const int relayPin = 2; //the base of the transistor attach to
String comdata = "";
int lastLength = 0;

void setup()
{
  pinMode(relayPin, OUTPUT);  //initialize the LED as output
  Serial.begin(9600);  // start serial port at 9600 bps:
  while (! Serial);
  Serial.println("Please input your command to control this Lamp:");  //print message on serial monitor
}

void loop()
{
 //read string from serial monitor
  if(Serial.available()>0)    // check if data has been sent from the computer
  {  
    comdata = "";
    while (Serial.available() > 0)  
    {        
      comdata += char(Serial.read());
      delay(2);
    }
    Serial.println(comdata);
  }
    if(comdata == "on")
    {
      digitalWrite(relayPin, HIGH);//turn the lamp on
    }
    else if(comdata == "off")
    {
      digitalWrite(relayPin, LOW);//turn the lamp off
    } 
    else
    {
      Serial.println("Please input correct command !");
      delay (20000);
    }
}
