/*
 * This example demonstrate code.
 * Compatible with "Espino32" by ThaiEasyElec.
 * Created : 2017.Aug.28
 * By Isaranu Janthong
 * IoTtweet Founder.
 * Visit us at : www.iottweet.com
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* -- Espino32 : OLED -- */
#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
  
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  /* initialize with the I2C addr 0x3D (for the 128x64) */
  display.display();
  delay(2000);

  /* clear screen */
  display.clearDisplay();

  /* Display message 1 */
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello Espino32 !");
  display.display();

  /* Display message 2 */
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("ThaiEasyElec na ja");
  display.display();
}  

void loop() {
  // code..
}
