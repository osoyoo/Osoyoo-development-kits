/*
* 发射红外线讯号
*     注意! 红外线发射器(IR LED)必须接到 pin 3, 不能接其它脚位
*/
#include <IRremote.h>                  // 引用 IRRemote 函式库
                            
const int buttonPin = 4;                 // 按键(pushbutton)接在 pin 4
int buttonState = 0;                     // 按键状态
                            
IRsend irsend;                          // 定义 IRsend 物件来发射红外线讯号
                            
void setup()
{
pinMode(buttonPin, INPUT);             // 把 buttonPin 设置成 INPUT
}
                            
void loop()
{
// 读取按键状态
buttonState = digitalRead(buttonPin);
                            
// 检查按键是否被按下(pressed)
// 有的话 buttonState 会是 HIGH
if (buttonState == HIGH) {
// 发射红外线讯号
irsend.sendNEC(0xFDB04F, 32);   // 记得换成你遥控器的红外线编码
}
}
