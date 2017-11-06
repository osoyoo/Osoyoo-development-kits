/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 *In this lesson, we will show how to use the 74HC595 to 
 *drive a single 7-segment LED display on the Osoyoo Uno board.
 *You should now see the 7-segment display from 0 to 9 and A to F.
 * Tutorial URL http://osoyoo.com/2017/07/18/arduino-lesson-74hc595-with-one-digit-7-segment-led-display/
 * CopyRight www.osoyoo.com
 */
const int latchPin = 12;//Pin connected to latchPin of 74HC595
const int clockPin = 8;//Pin connected to clockPin of 74HC595 
const int dataPin = 11; //Pin connected to dataPin of 74HC595 
//display 0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F
int datArray[16] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246, 238, 62, 156, 122, 158, 142};

void setup ()
{
  //set pins to output
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
}
void loop()
{
  //loop from 0 to 256
  for(int num = 0; num < 16; num++)
  {
    digitalWrite(latchPin,LOW); //ground latchPin and hold low for as long as you are transmitting
    shiftOut(dataPin,clockPin,MSBFIRST,datArray[num]);
    //return the latch pin high to signal chip that it 
    //no longer needataPin to listen for information
    digitalWrite(latchPin,HIGH); //pull the latchPin clockPin to save the data
    delay(1000); //wait for a second
  }
}
