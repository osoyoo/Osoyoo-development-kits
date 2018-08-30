/**
 * Derived from the Oak OLED_minimal example with additional
 * inspiration from the Adafruit GFX SSD1306 driver.
 *
 * OLED_minimal cites https://github.com/costonisp/ESP8266-I2C-OLED as
 * a major source as well.
 *
 * The MIT License (MIT)
 * Copyright (c) 2016 Brian Taylor
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "OakOLED.h"
#include <Arduino.h>

OakOLED::OakOLED()
  : Adafruit_GFX(OLED_WIDTH, OLED_HEIGHT) {
  memset(buffer, OLED_BUFFER_SIZE, 0);
}

void OakOLED::drawPixel(int16_t x, int16_t y, uint16_t c) {
  uint16_t idx = x + (y / 8) * OLED_WIDTH;
  uint8_t bit = y % 8;

  if(c) {
    buffer[idx] |= (1 << bit);
  } else {
    buffer[idx] &= ~(1 << bit);
  }
}


void OakOLED::display() {
  sendcommand(SSD1306_COLUMNADDR);
  sendcommand(0); // start at column == 0
  sendcommand(OLED_WIDTH - 1); // end at column == 127

  sendcommand(SSD1306_PAGEADDR);
  sendcommand(0); // start at page == 0
  sendcommand(7); // end at page == 7

  // we can go by 16s
  for(uint16_t ii = 0; ii < OLED_BUFFER_SIZE; ii+=16) {
    Wire.beginTransmission(OLED_address);
    Wire.write(0x40);

    for(uint16_t jj = 0; jj < 16; ++jj) {
      Wire.write(buffer[ii + jj]);
    }

    Wire.endTransmission();
  }
}

void OakOLED::begin() {
  Wire.begin();
  init_OLED();
  resetDisplay();
  clearDisplay();
}

//==========================================================//
// Resets display depending on the actual mode.
void OakOLED::resetDisplay(void)
{
  displayOff();
  clearDisplay();
  displayOn();
}

//==========================================================//
// Turns display on.
void OakOLED::displayOn(void)
{
  sendcommand(0xaf);        //display on
}

//==========================================================//
// Turns display off.
void OakOLED::displayOff(void)
{
  sendcommand(0xae);    //display off
}

//==========================================================//
// Clears the display by sendind 0 to all the screen map.
void OakOLED::clearDisplay(void)
{
  memset(buffer, 0, OLED_BUFFER_SIZE);
}

//==========================================================//
// Used to send commands to the display.
void OakOLED::sendcommand(unsigned char com)
{
  Wire.beginTransmission(OLED_address);     //begin transmitting
  Wire.write(0x80);                          //command mode
  Wire.write(com);
  Wire.endTransmission();                    // stop transmitting
}

//==========================================================//
// Inits oled and draws logo at startup
void OakOLED::init_OLED(void)
{
  sendcommand(0xae);    //display off
  sendcommand(0xa6);            //Set Normal Display (default)

  // Adafruit Init sequence for 128x64 OLED module
  sendcommand(0xAE);             //DISPLAYOFF
  sendcommand(0xD5);            //SETDISPLAYCLOCKDIV
  sendcommand(0x80);            // the suggested ratio 0x80
  sendcommand(0xA8);            //SSD1306_SETMULTIPLEX
  sendcommand(0x3F);
  sendcommand(0xD3);            //SETDISPLAYOFFSET
  sendcommand(0x0);             //no offset
  sendcommand(0x40 | 0x0);      //SETSTARTLINE
  sendcommand(0x8D);            //CHARGEPUMP
  sendcommand(0x14);
  sendcommand(0x20);             //MEMORYMODE
  sendcommand(0x00);             //0x0 act like ks0108

  //sendcommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
  sendcommand(0xA0);

  //sendcommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
  sendcommand(0xC0);

  sendcommand(0xDA);            //0xDA
  sendcommand(0x12);           //COMSCANDEC
  sendcommand(0x81);           //SETCONTRAS
  sendcommand(0xCF);           //
  sendcommand(0xd9);          //SETPRECHARGE
  sendcommand(0xF1);
  sendcommand(0xDB);        //SETVCOMDETECT
  sendcommand(0x40);
  sendcommand(0xA4);        //DISPLAYALLON_RESUME
  sendcommand(0xA6);        //NORMALDISPLAY

  clearDisplay();
  sendcommand(0x2e);            // stop scroll
  //----------------------------REVERSE comments----------------------------//
  sendcommand(0xa0);    //seg re-map 0->127(default)
  sendcommand(0xa1);    //seg re-map 127->0
  sendcommand(0xc8);
  delay(1000);
  //----------------------------REVERSE comments----------------------------//
  // sendcommand(0xa7);  //Set Inverse Display
  // sendcommand(0xae);   //display off
  sendcommand(0x20);            //Set Memory Addressing Mode
  sendcommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
  //  sendcommand(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)

  sendcommand(SSD1306_SETCONTRAST);
  sendcommand(0xff);
}
