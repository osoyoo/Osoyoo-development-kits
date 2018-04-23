#define BLYNK_PRINT Console
#include <Console.h>
#include <Bridge.h>
#include <SimpleTimer.h>
#include <BlynkSimpleYun.h>
#include <SPI.h>
#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <RFID.h> 
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";

SimpleTimer timer;
//************************I2C 1602 LCD Display
LiquidCrystal_I2C lcd(0x27,16,2);
//************************Servo/Keypad
Servo myservo;
BLYNK_WRITE(V3)
{
  myservo.write(param.asInt());
}

#define Password_Length 8 
//int signalPin = 12;
char Data[Password_Length]; 
char Master[Password_Length] = "123A456"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {47, 45, 43, 41}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {39, 37, 35, 33}; //connect to the column pinouts of the keypad
 
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

//************************RFID
RFID rfid(48,49);   //sda:pin48,rst:pin49
bool state=true;

//***********************DHT11
#define DHTPIN 5          // What digital pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301
DHT dht(DHTPIN, DHTTYPE);
//BlynkTimer timer;
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

//******************************Light sensor/Utrasonic sensor/ LED
#define echoPin 29
#define trigPin 27
int lightsensorPin = A0;    // select the input pin for the photoresistor
int light_val = 0;       // variable to store the value coming from the sensor 
int LED = 26;   // select the pin26 for the LED 

//****************************Widget LED
WidgetLED led1(V0); //register to virtual pin 0
WidgetLED led2(V1); //register to virtual pin 1
WidgetLED led3(V2); //register to virtual pin 2
WidgetLED led4(V4); //register to virtual pin 4

//************************************************************************smoke sensor mq2
//connect to A1
 int smoke_mq2 = A1;
int smoke = 0; 

//************************************************************************FLAME sensor
//connect to A2
int FlamePin = A2;  // This is for input pin
int Flame = 0;  // HIGH when FLAME Exposed

//************************************************************************BUZZER
int Buzzer = 31;//the pin of the active buzzer

//************************************************************************RGB1
const int redPin1 = 8; // R1 connected to digital pin 8 
const int greenPin1 = 9; // G1 to digital pin 9 
const int bluePin1 = 10; // B1 connected to digital pin 10 
//************************************************************************RGB2
const int redPin2 = 11; // R2 connected to digital pin 11 
const int greenPin2 = 12; // G2 to digital pin 12 
const int bluePin2 = 13; // B2 connected to digital pin 13 

//************************************************************************2 CHANNEL RELAYS
//the relays connect to
//int IN1 = 4;
//int IN2 = 3;

//************************************************************************Water sensor
//the signal pin connect to D7
int waterSensor = 7;
int waterValue = 0;

//************************************************************************PIR_HC_SR501
//Connect to D24
int pir_hc_sr501 = 24;
//int LED1 = 26;                // choose the pin for the LED
//int relayInput = 3;             // choose the pin for the relay
int pirState = LOW;             // we start, assuming no motion detected
int pirval = 0;                    // variable for reading the pin status

//rgb
void setColor(int red, int green, int blue)
{
  analogWrite(redPin1, red);
  analogWrite(greenPin1, green);
  analogWrite(bluePin1, blue);  
}


//keypad
void KEYPADSENSOR() {
   // Console.println("KEYPAD START!");

//char customKey;
  customKey = customKeypad.getKey();
  Console.println("KEYPAD START!");
  lcd.setCursor(0,0);
  lcd.print("Enter Password:");
  Console.println("Enter Password:");
  if (customKey){
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,1); 
    lcd.print(Data[data_count]); 
    Console.println(Data[data_count]);
    data_count++; 
    }

  if(data_count == Password_Length-1){
    lcd.clear();

    if(!strcmp(Data, Master)){
      setColor(0, 255, 0);  // set rgb1 green
      lcd.print("Correct");
      Console.println("Password Correct!");
      //digitalWrite(signalPin, HIGH); 
      myservo.write(90);//goes to 90 degrees
      //lcd.print("1602 LCD IS READY");
      //delay(1000);//wait for a second
      delay(5000);
      myservo.write(0);//back to 0 degrees 
      setColor(0, 0, 0);
      lcd.clear();
      }
    else{
      lcd.print("Incorrect");
      setColor(255, 0, 0);  // set rgb1 red
      delay(1000);
      setColor(0, 0, 0);
      } 
    lcd.clear();
    clearData();  
  }
}

void clearData(){
  while(data_count !=0)
  {
    Data[data_count--] = 0; 
  }
  return;
  lcd.clear();
}

//rfid

void RFIDSENSOR() {
    Console.println("RFID START!");
    unsigned char type[MAX_LEN];
    //to find the card
    if (rfid.isCard( )) {
      Console.println("Find the card!"); 
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
    //Select card, return card capacity (lock card, prevent most read), remove the line to read the card continuously
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
  //32 DA 94 10
  //Overhere, you need to use your own card message!!!
  if( id[0]==0x32 && id[1]==0xDA && id[2]==0x94 && id[3]==0x10 ) {
    Console.println("Hello Mary!");
    setColor(0, 255, 0);  // set rgb1 green
    myservo.write(90);//goes to 90 degrees
    // state=RelayStatus(state);
   delay(3000);
   setColor(0, 0, 0);
   myservo.write(0);//back to 0 degrees 
  } 
  //75 C0 D0 6
  //Overhere, you need to use your own card message!!!
  else if(id[0]==0x75 && id[1]==0xC0 && id[2]==0xD0 && id[3]==0x65) {
    Console.println("Hello MicroHao!");
    myservo.write(90);//goes to 90 degrees
    setColor(0, 255, 0);  // set rgb1 green
    Console.println("Open th door");
    delay(3000);
    myservo.write(0);//back to 0 degrees 
    setColor(0, 0, 0);
    Console.println("Close the door");
    //state=RelayStatus(state);
  }
  else{
    Console.println("Hello unkown guy!");
    setColor(255, 0, 0);  // set rgb1 red
    delay(500);
    setColor(0, 0, 0);
    delay(500);
    setColor(255, 0, 0);  // set rgb1 red
    delay(500);
    setColor(0, 0, 0);
  }
}

//dht

void DHTSENSOR ()
{
  Console.println("DHT START!");
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Console.println("Failed to read from DHT sensor!");
   // lcd.setCursor(0, 0);
   // lcd.print("Failed to read from DHT sensor!    : ");
   //   lcd.clear();
 delay(500);
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  delay(500);
}

//Light / Ultrasonic

void LIGHTSENSOR () {
  Console.println("LIGHT SENSOR AND HC-SR04 START!");
  float duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  
light_val = analogRead(lightsensorPin);   // read the value from the sensor  
    Console.print("light_val = ");
    Console.println(light_val);   //The serial will print the light value
    Console.print("Distance = ");
    Console.print(distance);
    Console.println(" cm");
  if(light_val>=400 && distance <= 10  )       // the point at which the state of LEDs change 
  {
    digitalWrite(LED, HIGH);
    delay(1000);
  }
  else
  {
    digitalWrite(LED, LOW);  //set LED off
  } 
}

//*********************************************  PIR HC-SR501
void PIRSENSOR () {
    Console.println("PIR SENSOR START!");
    pirval = digitalRead(pir_hc_sr501);  // read input value
    if (pirval == HIGH) {            // check if the input is HIGH
    //digitalWrite(LED, HIGH);  // turn LED ON
    led2.on(); // set LED on
    if (pirState == LOW) {
    // we have just turned on
    Console.println("Motion detected!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    // We only want to print on the output change, not state
    pirState = HIGH;
    }
  } else {
    led2.off();  //set LED off
    if (pirState == HIGH){
    // we have just turned of
    Console.println("Motion ended!");
    // We only want to print on the output change, not state
    pirState = LOW;
    }
  }
}

//*********************************************  Flame Sensor
void FLAMESENSOR () {
    Console.println("FLAME SENSOR START!");
    Flame = analogRead(FlamePin);
    Console.print("flame_val = ");
    Console.println(Flame);   //The serial will print the fire value
  if(Flame<=650)       // the point at which the state of LEDs change 
  {
    setColor(255, 0, 0); // turn the RGB LED red 
    delay(500); 
    setColor(0, 0, 0);
    delay(500); 
    setColor(255, 0, 0); // turn the RGB LED red 
    delay(500); 
    setColor(0, 0, 0);
    delay(500); 
    digitalWrite(Buzzer, HIGH);
    led3.on();

  }
  else
  {
    Console.println("No Fire!");
    digitalWrite(Buzzer, LOW);
    led3.off();
  } 
    //delay(2000);
}
//*********************************************  MQ2 Smoke sensor
void SMOKESENSOR () {
    Console.println("SMOKE SENSOR START!");
    smoke = analogRead(smoke_mq2);
    Console.print("smoke_val = ");
    Console.println(smoke);   //The serial will print the smoke value
  if(smoke>=240)       // the point at which the state of LEDs change 
  {
    setColor(255, 0, 0); // turn the RGB LED red 
    delay(2000); 
    setColor(0, 0, 0);
    delay(2000); 
    setColor(255, 0, 0); // turn the RGB LED red 
    delay(2000); 
    setColor(0, 0, 0);
    digitalWrite(Buzzer, HIGH);
    led1.on();
  }
  else
  {
    Console.println("No smoke!");
    digitalWrite(Buzzer, LOW);
    led1.off();
  } 
  //  delay(500);
}

//**********************void setup
void setup()
{
  Blynk.begin(auth); 
  Bridge.begin();
  Console.begin();
  SPI.begin();
  dht.begin();  
  myservo.attach(2);
  myservo.write(0);//back to 0 degrees 
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  rfid.init();
 // pinMode(led, OUTPUT);  
  //pinMode(relay,OUTPUT);
 // digitalWrite(relay,HIGH);
  // Setup a function to be called every second
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);      // declare LED as output
  pinMode(pir_hc_sr501, INPUT);     // declare sensor as input
  pinMode(Buzzer, OUTPUT);      // declare Buzzer as output
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);  

  timer.setInterval(1000, KEYPADSENSOR);
  timer.setInterval(1000, RFIDSENSOR);
  timer.setInterval(10000, DHTSENSOR);
  timer.setInterval(10000, FLAMESENSOR);
  timer.setInterval(10000, SMOKESENSOR);
  timer.setInterval(1000, LIGHTSENSOR);
  timer.setInterval(1000, PIRSENSOR);
  while (!Console);{}
    
}

//**********************void loop
void loop() 
{
  // this is where the "polling" occurs
  timer.run();
  Blynk.run();
}


