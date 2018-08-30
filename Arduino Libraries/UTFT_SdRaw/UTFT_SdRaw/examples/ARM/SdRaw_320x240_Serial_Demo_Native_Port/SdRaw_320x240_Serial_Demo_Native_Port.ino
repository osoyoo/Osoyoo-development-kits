// UTFT_SdRaw_320x240_Serial_Demo_Native_Port
// Copyright (C)2015 Graham Lawrence (GHLawrence2000). All Rights reserved.
// web: https://github.com/ghlawrence2000/UTFT_SdRaw
//
// This program is a demo of how to use Serial/SerialUSB source to draw an
// image with UTFT_SdRaw.
//
// The companion Processing sketch 'SdRaw_Serial_Uploader' can be found in
// the 'Extras' folder.
//
// This program requires the UTFT and SdFat libraries.
//

#include <UTFT.h>
#include <UTFT_SdRaw.h>

#include <SPI.h>
#include <SdFat.h>

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(CPLD, 25, 26, 27, 28);

UTFT_SdRaw myFiles(&myGLCD);

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();

  // Open Serial communications and wait for port to open:
  SerialUSB.begin(115200);
  while (!SerialUSB) ;
  // wait for Serial port to connect. Needed for Leonardo and DUE
}

void loop()
{
  while (!SerialUSB.available()) ;                  // Wait for Serial connection
  if (SerialUSB.find("HELLO\n")) {                  // Wait for host
    SerialUSB.println("OK");                        // Tell host we are ready
  }
  // loadS(port, x, y, xsize, ysize, buffermultiplier, invert colours)
  // port can be Serial / SerialUSB
  // x , y are the display coordinates
  // xsize , ysize are the expected image dimensions
  // buffer multiplier is used to potentially increase speed a little :-
  //      total buffer size = buffermultiplier * 2 * xsize
  // invert colours is used to swap the MSB/LSB if colour appears wrong
  myFiles.loadS(SerialUSB, 0, 0, 320, 240, 4 , 0);  // Draw Image
}
