
/*
 * This example demonstrate how to write data from your "Internet of Things" to IoTtweet dashboard
 * Compatible with "Espino32" by ThaiEasyElec.
 * Created : 2017.Aug.28
 * By Isaranu Janthong
 * IoTtweet Founder.
 * Visit us at : www.iottweet.com
 */

#include <IoTtweetESP32.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

/* -- Espino32 : IoTtweet -- */
const char *userid = "userid";                  //IoTtweet account user ID (6 digits, included zero pre-fix)
const char *key = "device-key";                 //IoTtweet registered device key in "MY IOT Garage"
const char *ssid = "ssid";                      //Your-WiFi-router-SSID
const char *password = "password";              //Your-WiFi-password

float data0, data1, data2, data3;               //Your sending data variable.
String message_1 = "Hello from Espino32";       //Your private tweet meassage to dashboard
String message_2 = "ThaieasyElec na ja :)";     //Your public tweet message to dashboard

IoTtweetESP32 myiot;                            //naming your devices

/* -- Espino32 : OLED -- */
#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM thermo16_glcd_bmp[] =
{ B00000001, B10000000,
  B00000010, B01000000,
  B00000100, B00100000,
  B11000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B11000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00001111, B11110000,
  B00001111, B11110000,
  B00000011, B11000000 };

    static const unsigned char PROGMEM drop16_glcd_bmp[] =
{ B00000001, B10000000,
  B00000001, B10000000,
  B00000001, B10000000,
  B00000011, B11000000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00111111, B11111100,
  B00111111, B11111100,
  B00111111, B11111100,
  B00111111, B11111100,
  B00111111, B11111100,
  B00011111, B11111000,
  B00001111, B11110000,
  B00000011, B11000000 };

      static const unsigned char PROGMEM blank16_glcd_bmp[] =
{ B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000 };


#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/* - OLED Display position offset. - */
int thermo_pos_x = 10, thermo_pos_y = 5;

/* -- Espino32 : DHT11 -- */
#define DHTPIN 10
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float h,t,f,hif,hic;
float t_offset = 0.0, h_offset = 0.0;

void setup() {
  
  Serial.begin(115200);
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  /* initialize with the I2C addr 0x3D (for the 128x64) */
  display.display();
  delay(2000);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Connecting to IoTtweet.com...");
  display.display();

  /* Get IoTtweet Library version */
  String libvers = myiot.getVersion();
  Serial.println("IoTtweet Library vesion : " + String(libvers));

  /* Connect WiFi */
  Serial.println("\nConnect wifi...");
  bool conn = myiot.begin(ssid,password);

  display.println("Lib.ver: " + String(libvers));
  if(!conn)
  {
      display.println("connect fails.");
    }else
    { display.println("connected !.");
      }

  display.display();
  delay(2000);
  display.clearDisplay();
}  

void loop() {

/* - DHT sensor reading - */
  float h = dht.readHumidity() + h_offset;
  float t = dht.readTemperature() + t_offset;
  float f = dht.readTemperature(true);

  delay(2000);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  /* - Draw icon -*/
  display.clearDisplay();
  display.drawBitmap(thermo_pos_x,thermo_pos_y, thermo16_glcd_bmp,16,16, WHITE);
  display.drawBitmap(thermo_pos_x,thermo_pos_y+30, drop16_glcd_bmp,16,16, WHITE);

  /* - Display Temperature - */
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(thermo_pos_x + 30, thermo_pos_y);
  display.println(String(t));
  
  /* - Display Humidity - */
  display.setCursor(thermo_pos_x + 30, thermo_pos_y + 30);
  display.println(String(h));

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(110, thermo_pos_y + 5);
  display.println("c.");
  display.setCursor(110, thermo_pos_y + 35);
  display.println("%RH");

  /* - Setup tweet message to display on IoTtweet dashboard - */
  message_1 = "อุณหภูมิตอนนี้ " + String(t) + " c";
  message_2 = "ความชื้นตอนนี้ " + String(h) + " %";

  /* - Send data from your iot to Dashboard - */
  String response = myiot.WriteDashboard(userid,key,t,h,data2,data3,message_1,message_2);
  
  display.setTextColor(WHITE);
  display.setCursor(17, 56);
  display.println("Uploading IoTtweet");
  display.display();
 
  delay(15000);

  /* OLED screen */
    display.setTextColor(BLACK);
    display.setCursor(17, 56);
    display.println("Uploading IoTtweet");
    display.display();
  
    display.setTextColor(WHITE);
    display.setCursor(40, 56);
    display.println("by Espino32");
    display.display();
    delay(2000);

    display.setTextColor(BLACK);
    display.setCursor(40, 56);
    display.println("by Espino32");
    display.display();

    display.setTextColor(WHITE);
    display.setCursor(40, 56);
    display.println("Read sensor..");
 
    display.display();
}
