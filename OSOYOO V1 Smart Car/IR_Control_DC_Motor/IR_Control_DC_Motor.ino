#include <IRremote.h>
#define IN1  2   //K1、K2 motor direction
#define IN2  4     //K1、K2 motor direction
#define ENA  5    // Needs to be a PWM pin to be able to control motor speed ENA
#define ENB  6    // Needs to be a PWM pin to be able to control motor speed ENA
#define IRPIN 3 //IR receiver Signal pin connect to Arduino pin 4

#define IR_ADVANCE       0x00FF18E7       //code from IR controller "▲" button
#define IR_BACK          0x00FF4AB5       //code from IR controller "▼" button
enum DN
{ 
  GO_ADVANCE, //go ahead
  GO_BACK,//go back
  DEF
}Drive_Num=DEF;
IRrecv IR(IRPIN);  //   IRrecv object  IR get code from IR remoter
decode_results IRresults;   
bool stopFlag = true;//set stop flag
bool JogFlag = false;
uint16_t JogTimeCnt = 0;
uint32_t JogTime=0;
/*motor control*/
void go_back(int t)  //motor rotate clockwise -->robot go ahead
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(t);
}
void go_ahead(int t) //motor rotate counterclockwise -->robot go back
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(t);
}
void go_stop() //motor brake -->robot stop
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

/*set motor speed */
void set_motorspeed(int lspeed,int rspeed) //change motor speed
{
  analogWrite(ENA,lspeed);//lspeed:0-255
  analogWrite(ENB,rspeed);//rspeed:0-255   
}
/**************detect IR code***************/
void do_IR_Tick()
{
  if(IR.decode(&IRresults))
  {
    if(IRresults.value==IR_ADVANCE)
    {
      Drive_Num=GO_ADVANCE;
    }
    else if(IRresults.value==IR_BACK)
    {
        Drive_Num=GO_BACK;
    }
    IRresults.value = 0;
    IR.resume();
  }
}
/**************car control**************/
void do_Drive_Tick()
{
    switch (Drive_Num) 
    {
      case GO_ADVANCE:
            set_motorspeed(255,255);go_ahead(20);JogFlag = true;JogTimeCnt = 3;JogTime=millis();break;//if GO_ADVANCE code is detected, then go advance
           case GO_BACK:
            set_motorspeed(255,255);go_back(20);JogFlag = true;JogTimeCnt = 1;JogTime=millis();break;//if GO_BACK code is detected, then backward
      default:break;
    }
    Drive_Num=DEF;
   //keep current moving mode for  100 millis seconds
    if(millis()-JogTime>=100)
    {
      JogTime=millis();
      if(JogFlag == true) 
      {
        stopFlag = false;
        if(JogTimeCnt <= 0) 
        {
          JogFlag = false; stopFlag = true;
        }
        JogTimeCnt--;
      }
      if(stopFlag == true) 
      {
        JogTimeCnt=0;
        go_stop();
      }
    }
}
void setup() {
  /******L298N******/
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);
 
  pinMode(IRPIN, INPUT); 
  digitalWrite(IRPIN, HIGH);  
  IR.enableIRIn();
}

void loop() {
  do_IR_Tick();
  do_Drive_Tick();
}
