/*
	文件：wifi_shield_updata.ino
	功能：利用wif拓展板上传温湿度数据到iot.doit.am，并且能通过云平台控制继电器
	硬件：arduino uno *1
		  Arduino UNO R3 Web Sever串口WiFi Siheld扩展板 *1
		  DHT11温湿度传感器模块  *1
		  继电器模块 *1
	服务器通讯协议：device_name 即平台上的device name 自定义，但需要一一对应
		订阅用户：
				用户发送：cmd=subscribe&topic=用户UID\r\n 
				服务器应答：cmd=subscribe&res=1
		上传数据：
				用户发送：cmd=upload&device_name=传感器名称&data=数据&uid=用户UID&key=用户KEY\r\n
				服务器应答：cmd=upload&res=1
		反向控制：
				服务器发送：cmd=publish&device_name=传感器名称&device_cmd=发送的数据\r\n

	作者：www.doit.am
	时间：2017-04-17
 */

#include <SoftwareSerial.h>
#include <dht.h>
dht DHT;

//用户信息,用户信息，需要填改写成自己的
#define UID "DavidGuo" //用户uid
#define KEY "ca18a87087463350dd70d53dbdd14ca7" //用户KEY

//device_name 即平台上的device name 自定义，但需要一一对应
#define DHT11_T "dht11_t" 
#define DHT11_H "dht11_h"
#define RELAYS  "relays"

//继电器控制指令 服务器平台发送的指令
#define RELAYS_ON  "on"
#define RELAYS_OFF "off"

//引脚
#define RELAYS_PIN 3 //继电器接UNO的gpio3
#define DHT11_PIN  2 //温湿度传感器接UNO的gpio2


//串口
#define DataSerial Serial
//软串,可以看调试信息
SoftwareSerial DebugSerial(10, 11);//rx tx

#define MAX_PACKETSIZE 512
//uartdata
char buffUart[MAX_PACKETSIZE];
unsigned int buffUartIndex=0;
unsigned long preUartTick=millis();

unsigned long updataTick=millis();

unsigned long subscribeTick=millis();
bool subscribeFlag =false;

/*
  检查并读取串口收到的数据
*/
void doUartTick()
{
  if (DataSerial.available())
  {
    buffUart[buffUartIndex++] = DataSerial.read();
    preUartTick = millis();
    if(buffUartIndex>=MAX_PACKETSIZE - 1){
      buffUartIndex = MAX_PACKETSIZE-2;
      preUartTick = preUartTick - 200;
    }
  }
  if(buffUartIndex>0 && (millis() - preUartTick>=100))
  {
    buffUart[buffUartIndex]=0x00;
    DataSerial.flush();
    parseUartPackage(buffUart,buffUartIndex);
    buffUartIndex = 0;
  }
}

/*
  * 发送数据到串口
 */
void sendUART(char *p,size_t len)
{
  DebugSerial.println("[UART send:]");
  DebugSerial.println(p);
  DataSerial.write(p,len);
}

/*
  * 发送数据到串口
 */
void sendUART(char *p)
{
  DebugSerial.println("[UART send:]");
  DebugSerial.println(p);
  DataSerial.print(p);
}

/*
	处理串口收到的数据
 */
void parseUartPackage(char *p,size_t len)
{
	DebugSerial.println("[UART Read:]");
	DebugSerial.println(p);
	
	char relaysStr[64]; memset(relaysStr,0,64);
	sprintf(relaysStr,"cmd=publish&device_name=%s&device_cmd=",RELAYS);
	
	if (strstr(p,relaysStr) != NULL)
    {
      	if (strstr(p+strlen(relaysStr),RELAYS_ON) != NULL)
	    {
	      digitalWrite(RELAYS_PIN, HIGH); //打开继电器
	      DebugSerial.println("open relays!");
	    }
	    else if (strstr(p+strlen(relaysStr),RELAYS_OFF) != NULL)
	    {
	      digitalWrite(RELAYS_PIN, LOW); //关闭继电器
	      DebugSerial.println("Close relays!");
	    }
    }
    else if (strstr(p,"cmd=upload&res=1") != NULL)
	{
	    DebugSerial.println("upload successful...");
	}
	else if (strstr(p,"cmd=subscribe&res=1") != NULL)
	{
	    subscribeFlag=true;
	    DebugSerial.println("Subscribe uid successful...");
	}

}

/*
	订阅用户，用于接受数据
 */
void doSubscribe()
{
	if( !subscribeFlag && millis() - subscribeTick > 1000 ) {
		subscribeTick=millis();
	    char suid[64]; memset(suid,0,64);
		sprintf(suid , "cmd=subscribe&topic=%s\r\n" , UID);
		sendUART(suid);
	}
}

/*
	初始化串口
 */
void initUart()
{
	DebugSerial.begin(9600);
	DataSerial.begin(9600);
}

/*
	初始化传感器IO
 */
void initGPIO()
{
	pinMode(RELAYS_PIN, OUTPUT);
}

/*
	读取传感器数据并上传
 */
void doUpdata()
{
	if(millis()-updataTick >5*1000){//5s上传一次数据
		updataTick=millis();

		int chk = DHT.read11(DHT11_PIN);
		switch (chk)
	  	{
	    	case DHTLIB_OK: 
	            DebugSerial.println("OK"); 
	            break;
	    	case DHTLIB_ERROR_CHECKSUM: 
	            DebugSerial.println("Checksum error"); 
	            break;
	    	case DHTLIB_ERROR_TIMEOUT: 
	            DebugSerial.println("Time out error"); 
	            break;
	    	default: 
	            DebugSerial.println("Unknown error"); 
	            break;
	  	}

	  	char str[128]; memset(str,0,128);
	    sprintf(str, "cmd=upload&device_name=%s&data=%d&device_name=%s&data=%d&uid=%s&key=%s\r\n", 
	    	DHT11_T , (int)DHT.temperature , DHT11_H , (int)DHT.humidity , UID , KEY);
	    
	    sendUART(str);
	}
}

//主函数
void setup()
{
  initUart();
  initGPIO();
          digitalWrite(RELAYS_PIN, HIGH); //打开继电器
          delay(1000);
                    digitalWrite(RELAYS_PIN, LOW); //打开继电器
          delay(1000);
           digitalWrite(RELAYS_PIN, HIGH); //打开继电器
          delay(1000);
                    digitalWrite(RELAYS_PIN, LOW); //打开继电器
          delay(1000);
}

void loop()
{
  doUartTick();
  doSubscribe();
  doUpdata();
}
