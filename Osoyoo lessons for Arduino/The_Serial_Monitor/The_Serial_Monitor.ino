
const int LED= 2; //the led pin attach to 
String comdata = "";
int lastLength = 0;

void setup()
{
  pinMode(LED, OUTPUT);  //initialize the LED as output
  Serial.begin(9600);  // start serial port at 9600 bps:
  while (! Serial);
  Serial.println("Please input your command to control this LED:");  //print message on serial monitor
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
      digitalWrite(LED, HIGH);//turn the led on
    }
    else if(comdata == "off")
    {
      digitalWrite(LED, LOW);//turn the led off
    } 
    else
    {                         
      digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);                       // wait for a second
      digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);   
      Serial.println("Please input correct command !");
    }
}

