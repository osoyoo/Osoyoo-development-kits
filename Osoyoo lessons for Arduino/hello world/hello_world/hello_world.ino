/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * This is the Arduino Hello Worls sketch.
 * A "Hello, World!" program is traditionally used to introduce novice programmers to a programming language.
 * After upload done,open the serial monitor,you will see the Arduino serial print "Hello World" every 5 second
 * Tutorial URL http://osoyoo.com/2017/06/23/arduino-lesson-hello-world/
 * CopyRight www.osoyoo.com
 */
void setup()
{
Serial.begin(9600);//Set the baudrate to 9600,make sure it's same as your software settings
                    //When accessing other devices (such as Bluetooth), we also have to agree
                    //with the baudrate of other devices.
}
void loop()
{
Serial.println("Hello World!");//The serial will print strings:“Hello World！”
delay(5000);// delay for 5 seconds(You can also change this numbers to config the delay time)
}
