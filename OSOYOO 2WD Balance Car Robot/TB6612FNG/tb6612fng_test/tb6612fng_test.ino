/****************************************************************************8
Osoyoo ：TB6612FNG test code
You can upload this sketch to the balance robot
*/
int STBY = 8; //set the EN pin
  
//Motor A
int PWMA = 9; //PWM output pin for Motor A
int AIN1 = 7; //positive pole for Motor A
int AIN2 = 6; //negative pole for Motor A
  
//Motor B  
int PWMB = 10; //PWM output pin for Motor B
int BIN1 = 13; //positive pole for Motor B
int BIN2 = 12; //negative pole for Motor B 
  
void setup(){  
  pinMode(STBY, OUTPUT);  
  
  pinMode(PWMA, OUTPUT);  
  pinMode(AIN1, OUTPUT);  
  pinMode(AIN2, OUTPUT);  
  
  pinMode(PWMB, OUTPUT);  
  pinMode(BIN1, OUTPUT);  
  pinMode(BIN2, OUTPUT);  
}  

void runset(int motor, int speed, int direction){  
  
  digitalWrite(STBY, HIGH); //EN PIN
  
  boolean Pin1 = LOW;  
  boolean Pin2 = HIGH;  
  
  if(direction == 1){  
    Pin1 = HIGH;  
    Pin2 = LOW;  
  }  
  
  if(motor == 1){  
    digitalWrite(AIN1, Pin1);  
    digitalWrite(AIN2, Pin2);  
    analogWrite(PWMA, speed);  
  }else{  
    digitalWrite(BIN1, Pin1);  
    digitalWrite(BIN2, Pin2);  
    analogWrite(PWMB, speed);  
  }  
}  
  
void loop(){  
  runset(1, 255, 1); //The motor A is moving forward at full speed.

  runset(2, 255, 1); //The motor B is moving forward at full speed. 
  
  delay(1000); //1秒 
  stop(); //STOP
  delay(250); //The two rounds go forward
  
  runset(1, 128, 0); //MOTOR A Turn left at half speed.
  runset(2, 128, 0); //MOTOR B Turn left at half speed. 
  
  delay(1000);  
  stop();  
  delay(250);  
}  
   
void stop(){  
  digitalWrite(STBY, LOW);   
}  
