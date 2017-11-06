#include <IRremote.h>   // Referring to the IRRemote function library, the header file 
                        //has defined PIN 3 as the signal output, so it can only connect 
                        //to PIN 3. If change, please change it in the header file                                              
IRsend irsend;                            
void setup()  
{  
}                                
void loop()  
{  
  irsend.sendNEC(0xFF02FD, 32);   //The code 'FF02FD' is the received infrared code  
                                  //if you need to use other functions, please 
                                  //use the receiving tube read the correct encoding value.  
  delay(2000);  
}  

