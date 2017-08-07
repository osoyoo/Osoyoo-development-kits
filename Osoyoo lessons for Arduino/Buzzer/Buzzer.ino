/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * In this lesson,we will show how to use a active buzzer to make some noise.
 * Tutorial URL: http://osoyoo.com/2017/07/03/arduino-lesson-buzzer/
 * CopyRight www.osoyoo.com
 */
int buzzer = 12;//the pin of the active buzzer
void setup()
{
pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output
}
void loop()
{
unsigned char i;
while(1)
{
//output an frequency
for(i=0;i<80;i++)
{
digitalWrite(buzzer,HIGH);
delay(1);//wait for 1ms
digitalWrite(buzzer,LOW);
delay(1);//wait for 1ms
}
//output another frequency
for(i=0;i<100;i++)
{
digitalWrite(buzzer,HIGH);
delay(2);//wait for 2ms
digitalWrite(buzzer,LOW);
delay(2);//wait for 2ms
}
}
}
