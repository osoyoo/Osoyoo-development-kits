#include "IRremote.h"
/*-----( Declare Constants )-----*/
int receiver = 2; // pin 1 of IR receiver to Arduino digital pin 2
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
/*-----( Declare Variables )-----*/

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  Serial.println("IR Receiver Raw Data + Button Decode Test");
  irrecv.enableIRIn(); // Start the receiver
}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
//    Serial.println(results.value, HEX);  UN Comment to see raw values
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
void translateIR() // takes action based on IR code received
// describing Car MP3 IR codes 
{
  switch(results.value)
  {
  case 0xFFA25D:  
    Serial.println(" CH-            "); 
    break;
  case 0xFF629D:  
    Serial.println(" CH             "); 
    break;
  case 0xFFE21D:  
    Serial.println(" CH+            "); 
    break;
  case 0xFF22DD:  
    Serial.println(" PREV           "); 
    break;
  case 0xFF02FD:  
    Serial.println(" NEXT           "); 
    break;
  case 0xFFC23D:  
    Serial.println(" PLAY/PAUSE     "); 
    break;
  case 0xFFE01F:  
    Serial.println(" VOL-           "); 
    break;
  case 0xFFA857:  
    Serial.println(" VOL+           "); 
    break;
  case 0xFF906F:  
    Serial.println(" EQ             "); 
    break;
  case 0xFF6897:  
    Serial.println(" 0              "); 
    break;
  case 0xFF9867:  
    Serial.println(" 100+           "); 
    break;
  case 0xFFB04F:  
    Serial.println(" 200+           "); 
    break;
  case 0xFF30CF:  
    Serial.println(" 1              "); 
    break;
  case 0xFF18E7:  
    Serial.println(" 2              "); 
    break;
  case 0xFF7A85:  
    Serial.println(" 3              "); 
    break;
  case 0xFF10EF:  
    Serial.println(" 4              "); 
    break;
  case 0xFF38C7:  
    Serial.println(" 5              "); 
    break;
  case 0xFF5AA5:  
    Serial.println(" 6              "); 
    break;
  case 0xFF42BD:  
    Serial.println(" 7              "); 
    break;
  case 0xFF4AB5:  
    Serial.println(" 8              "); 
    break;
  case 0xFF52AD:  
    Serial.println(" 9              "); 
    break;
  default: 
    Serial.println(" other button   ");
  }
  delay(500);
} //END translateIR

