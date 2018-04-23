#include <SPI.h>
#include <RFID.h> 
#include <Console.h>
RFID rfid(48,49);   //D10--读卡器SS引脚、D5--读卡器RST引脚

int LED = 26;

bool state=true;
void setup()
{
  Bridge.begin();
  Console.begin();
  SPI.begin();
  rfid.init();
  pinMode(LED, OUTPUT);  
  digitalWrite(LED,LOW);
  while (!Console);
}

void loop()
{
  unsigned char type[MAX_LEN];
  //Findeed the card
  if (rfid.isCard( )) {
    Console.println("Find the card!"); 
    ;
    // Show card type
    ShowCardType(type);
    //Read card sequence number
    if (rfid.readCardSerial()) {
      Console.print("The card's number is  : ");
      Console.print(rfid.serNum[0],HEX);
      Console.print(rfid.serNum[1],HEX);
      Console.print(rfid.serNum[2],HEX);
      Console.print(rfid.serNum[3],HEX);
      Console.print(rfid.serNum[4],HEX);
      Console.println(" ");
      ShowUser(rfid.serNum);
    }
    //Select card, return card capacity (lock card,prevent most read),
    //remove the line to read the card continuously
    Console.println(rfid.selectTag(rfid.serNum));
  }

  rfid.halt();
}

void ShowCardType( unsigned char* type)
{
  Console.print("Card type: ");
  if(type[0]==0x04&&type[1]==0x00) 
    Console.println("MFOne-S50");
  else if(type[0]==0x02&&type[1]==0x00)
    Console.println("MFOne-S70");
  else if(type[0]==0x44&&type[1]==0x00)
    Console.println("MF-UltraLight");
  else if(type[0]==0x08&&type[1]==0x00)
    Console.println("MF-Pro");
  else if(type[0]==0x44&&type[1]==0x03)
    Console.println("MF Desire");
  else
    Console.println("Unknown");
}

void ShowUser( unsigned char* id)
{
  //Read the card information with your program first, 
  //and then change it here
  //32 DA 94 10 6C
  if( id[0]==0x32 && id[1]==0xDA && id[2]==0x94 && id[3]==0x10 ) {
    Console.println("Hello Mary!");
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000); 
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  } 
  //75 C0 D0 65 0 
  else if(id[0]==0x75 && id[1]==0xC0 && id[2]==0xD0 && id[3]==0x65) {
    Console.println("Hello MicroHao!");
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000); 
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  }
  else{
    Console.println("Hello unkown guy!");
  }
}

