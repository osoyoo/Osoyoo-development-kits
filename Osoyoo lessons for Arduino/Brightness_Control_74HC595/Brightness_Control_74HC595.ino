int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int outputEnablePin = 3;
 
byte leds = 0;
 
void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT); 
}
 
void loop() 
{
  setBrightness(255);
  leds = 0;
  updateShiftRegister();
  delay(500);
  for (int i = 0; i < 8; i++)
  {
    bitSet(leds, i);
    updateShiftRegister();
    delay(500);
  }
  for (byte b = 255; b > 0; b--)
  {
    setBrightness(b);
    delay(50);
  }
}
 
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
 
void setBrightness(byte brightness) // 0 to 255
{
  analogWrite(outputEnablePin, 255-brightness);
}
