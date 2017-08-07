const int red1Pin= 5; //red1 led attach to 
const int yellow1Pin =6 ; //yellow1 led attach to 
const int green1Pin= 7; //green1 led attach to 

const int red2Pin= 2; //red2 led attach to 
const int yellow2Pin =3 ; //yellow2 led attach to 
const int green2Pin= 4; //green2 led attach to 

const int STcp = 12;//Pin connected to ST_CP of 74HC595
const int SHcp = 8;//Pin connected to SH_CP of 74HC595 
const int DS = 11; //Pin connected to DS of 74HC595 
//display 1,2,3,4,5,6,7,8,9
int datArray[16] = {
  96, 218, 242, 102, 182, 190, 224, 254, 246};

void setup()
{
  pinMode(red1Pin, OUTPUT);  //set the redPin as an output
  pinMode(yellow1Pin, OUTPUT);  //set the yellowPin as an output
  pinMode(green1Pin, OUTPUT);  //set the greenPin as an output

  pinMode(red2Pin, OUTPUT);  //set the redPin as an output
  pinMode(yellow2Pin, OUTPUT);  //set the yellowPin as an output
  pinMode(green2Pin, OUTPUT);  //set the greenPin as an output
  //set pins to output
  pinMode(STcp,OUTPUT);
  pinMode(SHcp,OUTPUT);
  pinMode(DS,OUTPUT);
  Serial.begin(9600);  // start serial port at 9600 bps:
}

void loop()
{
  State1();
  State2(); 
}
void State1()
{
  digitalWrite(red1Pin,HIGH);   //turn on a red led
  for(int num = 8; num >=0; num--)   //display 9-1 and turn on a green led
  {
    digitalWrite(green2Pin,HIGH);
    digitalWrite(STcp,LOW); //ground ST_CP and hold low for transmitting
    shiftOut(DS,SHcp,MSBFIRST,datArray[num]);
    digitalWrite(STcp,HIGH); //pull the ST_CPST_CP to save the data
    delay(1000); //wait for a second
  }
  digitalWrite(green2Pin,LOW);   //turn off the green led
  for(int num = 2 ;num >=0; num--)   //diaplay 3 to 1 and turn on the yellow led 
  {
    digitalWrite(yellow2Pin,HIGH);
    digitalWrite(STcp,LOW); //ground ST_CP and hold low for transmitting
    shiftOut(DS,SHcp,MSBFIRST,datArray[num]);
    digitalWrite(STcp,HIGH); //pull the ST_CPST_CP to save the data
    delay(1000); //wait for a second
  }
  digitalWrite(yellow2Pin,LOW);  //turn off the yellow led
  digitalWrite(red1Pin,LOW);  //the red led finally turn off
}
void State2()
{
  digitalWrite(red2Pin,HIGH);
  for(int num = 8; num >=0; num--)
  {
    digitalWrite(green1Pin,HIGH);
    digitalWrite(STcp,LOW); //ground ST_CP and hold low for as long as you are transmitting
    shiftOut(DS,SHcp,MSBFIRST,datArray[num]);
    digitalWrite(STcp,HIGH); //pull the ST_CPST_CP to save the data
    delay(1000); //wait for a second
  }
  digitalWrite(green1Pin,LOW);
  for(int num = 2 ;num >=0; num--)
  {
    digitalWrite(yellow1Pin,HIGH);
    digitalWrite(STcp,LOW); //ground ST_CP and hold low for as long as you are transmitting
    shiftOut(DS,SHcp,MSBFIRST,datArray[num]);
    digitalWrite(STcp,HIGH); //pull the ST_CPST_CP to save the data
    delay(1000); //wait for a second
  }
  digitalWrite(yellow1Pin,LOW);
  digitalWrite(red2Pin,LOW);
}
