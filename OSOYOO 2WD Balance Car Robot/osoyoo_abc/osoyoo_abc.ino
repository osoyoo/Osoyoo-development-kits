#include <PinChangeInt.h>
#include <MsTimer2.h>
//Speed PID control realized by encoder counting
#include <BalanceCar.h>
#include <KalmanFilter.h>
//I2Cdev、MPU6050 libraries need to be installed in the Arduino Libraries folder.
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu; //Instantiate a MPU6050 object with the object name MPU.
BalanceCar balancecar;
KalmanFilter kalmanfilter;
int16_t ax, ay, az, gx, gy, gz;
//TB6612FNG drive module control signal
#define IN1M 7
#define IN2M 6
#define IN3M 13
#define IN4M 12
#define PWMA 9
#define PWMB 10
#define STBY 8
//Encoder count signal
#define PinA_left 2  //Interrupt 0
#define PinA_right 4 //Interrupt 1
//Declare custom variables

int time;
byte inByte; //Serial receive byte
int num;
double Setpoint;//Angle DIP setting point, input, output
double Setpoints, Outputs = 0;//Speed DIP setting point, input, output
double kp = 40, ki = 0.0, kd = 0.6;//You need to modify the parameters, angle PD control.
double kp_speed =5.20, ki_speed = 0.25, kd_speed = 0.0;// You need to modify the parameters, Speed PD control.
double kp_turn = 23, ki_turn = 0, kd_turn = 0.3;//Rotating PD settings
const double PID_Original[6] = {40, 0.0, 0.6, 5.20, 0.25, 0.0}; //Restore default PID parameters
//Steering PID parameters
double setp0 = 0, dpwm = 0, dl = 0; //Angle balance point, PWM difference, dead zone, PWM1, PWM2
float value;
/********************angle data*********************/
float Q;
float Angle_ax; //Tilt angle calculated from acceleration
float Angle_ay;
float K1 = 0.05; // Weight of accelerometer values
float angle0 = 0.00; //Mechanical balance angle
int slong;
/***************Kalman_Filter*********************/
float Q_angle = 0.001, Q_gyro = 0.005; //Confidence of angle data and confidence of angular velocity data
float R_angle = 0.5 , C_0 = 1;
float timeChange = 5; //Filtering time interval millisecond
float dt = timeChange * 0.001; //Note: the value of DT is filter sampling time.
/***************Kalman_Filter*********************/
/******************** speed count ************/
volatile long count_right = 0;//The volatile lon type is used to ensure that the value is valid when the external interrupt pulse count value is used in other functions
volatile long count_left = 0;//The volatile lon type is used to ensure that the value is valid when the external interrupt pulse count value is used in other functions
int speedcc = 0;
/********************Pulse calculation********************/
int lz = 0;
int rz = 0;
int rpluse = 0;
int lpluse = 0;
int sumam;
/********************Steering and rotation parameters********************/
int turncount = 0; //Steering intervention time calculation
float turnoutput = 0;
/********************Bluetooth control********************/
#define run_car     '1'//forward
#define back_car    '2'//backward
#define left_car    '3'//turn left
#define right_car   '4'//turn right
#define stop_car    '0'//stop
/********************Enumeration of car running status********************/
enum {
  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT
} enCarState;
int incomingByte = 0;          // Received data byte
String inputString = "";         // Used to store received contents.
boolean newLineReceived = false; // Previous data end flag
boolean startBit  = false;  //Protocol start sign

int g_carstate = enSTOP; //  1forward 2backward 3left 4right 0stop
String returntemp = ""; //Store return value
boolean g_autoup = false;
int g_uptimes = 5000;
int front = 0;//Forward variable
int back = 0;//Backward variable
int turnl = 0;//turn left
int turnr = 0;//turn right
int spinl = 0;//Left rotation
int spinr = 0;//right rotation
int bluetoothvalue;//Bluetooth control
/********************Ultrasonic speed********************/
int chaoshengbo = 0;
int tingzhi = 0;
int jishi = 0;
/********************Pulse calculation********************/
void countpluse()
{
  lz = count_left;
  rz = count_right;
  count_left = 0;
  count_right = 0;
  lpluse = lz;
  rpluse = rz;
  if ((balancecar.pwm1 < 0) && (balancecar.pwm2 < 0))//The direction of the trolley motion is judged to be backward (PWM is the motor voltage is negative) and the number of pulses is negative.
  {
    rpluse = -rpluse;
    lpluse = -lpluse;
  }
  else if ((balancecar.pwm1 > 0) && (balancecar.pwm2 > 0))//The direction of the trolley motion is judged to be forward (PWM is the motor voltage is positive) and the number of pulses is negative.
  {
    rpluse = rpluse;
    lpluse = lpluse;
  }
  else if ((balancecar.pwm1 < 0) && (balancecar.pwm2 > 0))//The direction of the trolley motion is judged to be forward (PWM is the motor voltage is positive) and the number of pulses is negative.
  {
    rpluse = rpluse;
    lpluse = -lpluse;
  }
  else if ((balancecar.pwm1 > 0) && (balancecar.pwm2 < 0))//The direction of trolley motion determines that the number of left rotation and right pulse is negative, and the number of left pulses is positive.
  {
    rpluse = -rpluse;
    lpluse = lpluse;
  }
  //judgement
  balancecar.stopr += rpluse;
  balancecar.stopl += lpluse;
  //When every 5ms enters interruption, the number of pulses is superimposed.
  balancecar.pulseright += rpluse;
  balancecar.pulseleft += lpluse;
  sumam = balancecar.pulseright + balancecar.pulseleft;
}
/********************Angle PD********************/
void angleout()
{
  balancecar.angleoutput = kp * (kalmanfilter.angle + angle0) + kd * kalmanfilter.Gyro_x;//PD angle control
}
/********************Interrupt timing 5ms timing interrupt********************/
void inter()
{
  sei();//enable interrupt
  countpluse();//Pulse superposition subfunction
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);//IIC get MPU6050 six axis data  ax ay az gx gy gz
  kalmanfilter.Angletest(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gyro, R_angle, C_0, K1);//Obtaining angle angle and Kaman filtering
  angleout();//angle PD control
  speedcc++;
  if (speedcc >= 10)//50ms enter apeed control
  {
    Outputs = balancecar.speedpiout(kp_speed, ki_speed, kd_speed, front, back, setp0);
    speedcc = 0;
  }
  turncount++;
  if (turncount > 4)//20ms Enter rotary control
  {
    turnoutput = balancecar.turnspin(turnl, turnr, spinl, spinr, kp_turn, kd_turn, kalmanfilter.Gyro_z);//Rotation subfunction
    turncount = 0;
  }
  balancecar.posture++;
  balancecar.pwma(Outputs, turnoutput, kalmanfilter.angle, kalmanfilter.angle6, turnl, turnr, spinl,\
                  spinr, front, back, kalmanfilter.accelz, IN1M, IN2M, IN3M, IN4M, PWMA, PWMB);//Total PWM output of car
}
/********************Interrupt timing 5ms timing interrupt********************/
void SendAutoUp()
{
  g_uptimes --;
  if ((g_autoup == true) && (g_uptimes == 0))
  {
    //Automatic report
    String CSB, VT;
    char temp[10]={0};
    float fgx;
    float fay;
    float leftspeed;
    float rightspeed;
    fgx = gx;//Passing to local variables
    fay = ay;//Passing to local variables
    leftspeed = balancecar.pwm1;
    rightspeed = balancecar.pwm2;
    double Gx = (double)((fgx - 128.1f) / 131.0f);//Angle conversion
    double Ay = ((double)fay / 16384.0f) * 9.8f;
   if(leftspeed > 255 || leftspeed < -255)
      return;
   if(rightspeed > 255 || rightspeed < -255)
      return;
   if((Ay < -20) || (Ay > 20))
      return;
   if((Gx < -3000) || (Gx > 3000))
      return;  
    returntemp = "";
    memset(temp, 0x00, sizeof(temp));
    //sprintf(temp, "%3.1f", leftspeed);
    dtostrf(leftspeed, 3, 1, temp);  // Equivalent to %3.2f
    String LV = temp;
    memset(temp, 0x00, sizeof(temp));
    //sprintf(temp, "%3.1f", rightspeed);
    dtostrf(rightspeed, 3, 1, temp);  // Equivalent to %3.1f
    String RV = temp;
    memset(temp, 0x00, sizeof(temp));
    //sprintf(temp, "%2.2f", Ay);
    dtostrf(Ay, 2, 2, temp);  // Equivalent to %2.2f
    String AC = temp;
    memset(temp, 0x00, sizeof(temp));
    //sprintf(temp, "%4.2f", Gx);
    dtostrf(Gx, 4, 2, temp);  // Equivalent to %4.2f
    String GY = temp;
    CSB = "0.00";
    VT = "0.00";
    //AC =
    returntemp = "$LV" + LV + ",RV" + RV + ",AC" + AC + ",GY" + GY + ",CSB" + CSB + ",VT" + VT + "#";
    Serial.print(returntemp); //Return protocol packet
  }
  if (g_uptimes == 0)
      g_uptimes = 5000;
}
/********************Initialization settings********************/
void setup() {
  // TB6612FNGN Driver module control signal initialization
  pinMode(IN1M, OUTPUT);//Control the direction of motor 1, 01 is positive, 10 is reverse.
  pinMode(IN2M, OUTPUT);
  pinMode(IN3M, OUTPUT);//Control the direction of motor 2, 01 is positive, 10 is reverse.
  pinMode(IN4M, OUTPUT);
  pinMode(PWMA, OUTPUT);//PWM of left motor
  pinMode(PWMB, OUTPUT);//PWM of right motor
  pinMode(STBY, OUTPUT);//enable TB6612FNG
  //Initializing motor drive module
  digitalWrite(IN1M, 0);
  digitalWrite(IN2M, 1);
  digitalWrite(IN3M, 1);
  digitalWrite(IN4M, 0);
  digitalWrite(STBY, 1);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  //Speed dial input
  pinMode(PinA_left, INPUT);  
  pinMode(PinA_right, INPUT);
  // Add I2C bus
  Wire.begin();//Add I2C bus sequence
  Serial.begin(9600);//Open the serial port and set the baud rate of 115200.
  delay(1500);
  mpu.initialize();//Initialization MPU6050
  delay(2);
  balancecar.pwm1 = 0;
  balancecar.pwm2 = 0;
  //5ms timing interrupt settings, using timer2. note: using timer2 will have an impact on PWM output of PIN3 pin11.
  //Because PWM uses a timer to control the duty cycle, it is important to look at the pin port corresponding to the timer when using timer.
  MsTimer2::set(5, inter);
  MsTimer2::start();
}
/********************Reset PID parameters********************/
void ResetPID()
{
  kp = PID_Original[0];
  ki =  PID_Original[1];
  kd =  PID_Original[2];//Parameters that you need to modify
  kp_speed =  PID_Original[3];
  ki_speed =  PID_Original[4];
  kd_speed =  PID_Original[5];//Parameters that you need to modify
}
/********************Reset car condition********************/
void ResetCarState()
{
  turnl = 0; 
  turnr = 0;  
  front = 0; 
  back = 0; 
  spinl = 0; 
  spinr = 0; 
  turnoutput = 0;
}
/********************Main cycle********************/
void loop() 
{
  String returnstr = "$0,0,0,0,0,0,0,0,0,0,0,0cm,8.2V#";//Default sending
  //In the main function, the cycle detection and the superposition pulse measurement are used to determine the speed of the car. The change of the level can increase the pulse number of the motor and ensure the accuracy of the car.
  attachInterrupt(0, Code_left, CHANGE);
  attachPinChangeInterrupt(PinA_right, Code_right, CHANGE);
  if (newLineReceived)
  {
    switch (inputString[1])
    {
      case run_car:   g_carstate = enRUN;   break;
      case back_car:  g_carstate = enBACK;  break;
      case left_car:  g_carstate = enLEFT;  break;
      case right_car: g_carstate = enRIGHT; break;
      case stop_car:  g_carstate = enSTOP;  break;
      default: g_carstate = enSTOP; break;
     }
    //Determine whether the protocol has packet loss.
    if (inputString[3] == '1' && inputString.length() == 21)//pan left
    {
      g_carstate = enTLEFT;
      //Serial.print(returnstr);
      }
    else if (inputString[3] == '2' && inputString.length() == 21)//pan right
     {
      g_carstate = enTRIGHT;
      // Serial.print(returnstr);
     }
    if (inputString[5] == '1') //inquire PID
      {
        char charkp[7], charkd[7], charkpspeed[7], charkispeed[7];
        dtostrf(kp, 3, 2, charkp);//Amount to %3.2f
        dtostrf(kd, 3, 2, charkd);//Amount to %3.2f
        dtostrf(kp_speed, 3, 2, charkpspeed);//Amount to %3.2f
        dtostrf(ki_speed, 3, 2, charkispeed);//Amount to %3.2f
        String strkp = charkp; String strkd = charkd; 
        String strkpspeed = charkpspeed; String strkispeed = charkispeed;
        returntemp = "$0,0,0,0,0,0,AP" + strkp + ",AD" + strkd + ",VP" + strkpspeed + ",VI" + strkispeed + "#";
        Serial.print(returntemp); //Return protocol packet
      }
      else if (inputString[5] == '2')//recover PID
      {
        ResetPID();
        Serial.print("$OK#");//Return protocol packet
      }
      if (inputString[7] == '1')//Automatic report
      {
        g_autoup = true;
        Serial.print("$OK#");//Return protocol packet
      }
      else if (inputString[7] == '2')//Stop reporting automatically
      {
        g_autoup = false;
        Serial.print("$OK#"); //Return protocol packet
      }
      if (inputString[9] == '1') //Angle loop update $0,0,0,0,1,1,AP23.54,AD85.45,VP10.78,VI0.26#
      {
        int i = inputString.indexOf("AP");
        int ii = inputString.indexOf(",", i);
        if(ii > i)
        {
          String m_skp = inputString.substring(i + 2, ii);
          m_skp.replace(".", "");
          int m_kp = m_skp.toInt();
          kp = (double)( (double)m_kp / 100.0f);
        }
        i = inputString.indexOf("AD");
        ii = inputString.indexOf(",", i);
        if(ii > i)
        {
          //ki = inputString.substring(i+2, ii);
          String m_skd = inputString.substring(i + 2, ii);
          m_skd.replace(".", "");
          int m_kd = m_skd.toInt();
          kd = (double)( (double)m_kd / 100.0f);
        }
        Serial.print("$OK#");//Return protocol packet
      }
      if (inputString[11] == '1') //Speed loop update
      {
        int i = inputString.indexOf("VP");
        int ii = inputString.indexOf(",", i);
        if(ii > i)
        {
          String m_svp = inputString.substring(i + 2, ii);
          m_svp.replace(".", "");
          int m_vp = m_svp.toInt();
          kp_speed = (double)( (double)m_vp / 100.0f);
        }
        i = inputString.indexOf("VI");
        ii = inputString.indexOf("#", i);
        if(ii > i)
        {
          String m_svi = inputString.substring(i + 2, ii);
          m_svi.replace(".", "");
          int m_vi = m_svi.toInt();
          ki_speed = (double)( (double)m_vi / 100.0f);
          Serial.print("$OK#"); //Return protocol packet
        }
      }
      //Restore the default
      inputString = "";   // clear the string
      newLineReceived = false;
    } 
a:    switch (g_carstate)
    {
      case enSTOP:turnl = 0;turnr = 0;front = 0;back = 0;spinl = 0;spinr = 0;turnoutput = 0;break;
      case enRUN:ResetCarState();front = 250;break;
      case enLEFT:turnl = 1;break;
      case enRIGHT:turnr = 1;break;
      case enBACK:ResetCarState();back = -250;break;
      case enTLEFT:spinl = 1;break;
      case enTRIGHT:spinr = 1;break;
      default:front = 0;back = 0;turnl = 0;turnr = 0;spinl = 0;spinr = 0;turnoutput = 0;break;
    }
   //Increased automatic reporting
  SendAutoUp();
}
/********************Pulse interruption service function********************/
/********************Left speed dial count********************/
void Code_left() 
{
  count_left ++;
} 
/********************右测速码盘计数********************/
void Code_right() 
{
  count_right ++;
}
//SerialEvent () is a new feature in IDE1.0 and later versions. It's not clear why most people don't want to use it. It's the same as interrupt.
int num1 = 0;
void serialEvent()
{
  while (Serial.available())
  {
    incomingByte = Serial.read();//Read byte by byte, and the next sentence is read into an array of strings to form a complete packet
    if (incomingByte == '$')
    {
      num1 = 0;
      startBit = true;
    }
    if (startBit == true)
    {
      num1++;
      inputString += (char) incomingByte;// Full duplex serial ports can be added without delay, and half duplex will be added.//
    }
    if (startBit == true && incomingByte == '#')
    {
      newLineReceived = true;
      startBit = false;
    }
    
    if(num1 >= 80)
    {
      num1 = 0;
      startBit = false;
      newLineReceived = false;
      inputString = "";
    }	
  }
}
/********************备份********************/
#if 0
char chartemp[7];
dtostrf(ax, 3, 2, chartemp);//Amount to %3.2f
String strax = chartemp;
strax = "\nax:" + strax;

memset(chartemp, 0x00, 7);
dtostrf(ay, 3, 2, chartemp);//Amount to %3.2f
String stray = chartemp;
stray = "\nay:" + stray;

memset(chartemp, 0x00, 7);
dtostrf(az, 3, 2, chartemp);//Amount to %3.2f
String straz = chartemp;
straz = "\naz:" + straz;

memset(chartemp, 0x00, 7);
dtostrf(gx, 3, 2, chartemp);// Amount to%3.2f
String strgx = chartemp;
strgx = "\ngx:" + strgx;

memset(chartemp, 0x00, 7);
dtostrf(gy, 3, 2, chartemp);//Amount to %3.2f
String strgy = chartemp;
strgy = "\ngy:" + strgy;

memset(chartemp, 0x00, 7);
dtostrf(gz, 3, 2, chartemp);// Amount to %3.2f
String strgz = chartemp;
strgz = "\ngz:" + strgz;
Serial.print(strax + stray + straz + strgx + strgy + strgz);//Return protocol packet
#endif

