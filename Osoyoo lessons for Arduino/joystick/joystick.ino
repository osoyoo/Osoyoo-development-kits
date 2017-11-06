/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *This is an example Arduino sketch which sets up the 
 *microcontroller to read the inputs, and then continuously 
 *prints the values to the serial monitor.
 * Tutorial URL http://osoyoo.com/?p=11412
 * CopyRight www.osoyoo.com
 */
const int xPin = A0; //X attach to A0
const int yPin = A1; //Y attach to A1
const int swPin = 2; //Bt attach to digital 8
void setup()
{
  pinMode(swPin,INPUT); //set btpin as INPUT 
  digitalWrite(swPin, HIGH); //and HIGH
  Serial.begin(9600); //initialize serial monitor
}
void loop()
{
  Serial.print("X: "); //print "X: "
  Serial.print(analogRead(xPin),DEC); //read the value of A0 and print it in decimal
  Serial.print("|Y: "); //print "|Y: "
  Serial.print(analogRead(yPin),DEC); //read the value of A1 and print it in decimal
  Serial.print("|Z: "); //print "|Z: "
  Serial.println(digitalRead(swPin)); //read the value of pin8 and print it
  delay(500); //delay 500ms
}
