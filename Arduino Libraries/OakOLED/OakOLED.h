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

#ifndef OAKOLED_H
#define OAKOLED_H

#include <Wire.h>

#include <Adafruit_GFX.h>

#define OLED_address  0x3c
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_BUFFER_SIZE (OLED_WIDTH * OLED_HEIGHT / 8)

#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_SETCONTRAST 0x81

class OakOLED : public Adafruit_GFX {
  public:

  OakOLED();
  virtual void drawPixel(int16_t, int16_t, uint16_t);
  void display();

  void begin();

  void resetDisplay(void);

  void displayOn(void);

  void displayOff(void);

  void clearDisplay(void);

  void sendcommand(unsigned char com);

  uint8_t buffer[OLED_BUFFER_SIZE];

  private:


  //==========================================================//
  // Inits oled and draws logo at startup
  void init_OLED(void);

};

#endif
