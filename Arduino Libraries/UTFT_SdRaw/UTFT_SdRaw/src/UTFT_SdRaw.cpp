/*
UTFT_SdRaw.cpp - Add-on library to integrate UTFT and SdFat.
Copyright (C)2014 Graham Lawrence. Basic functionality of this library
is based on UTFT_tinyFAT by Henning Karlsen. All right reserved

This library is free software; you can redistribute it and/or
modify it under the terms of the CC BY-NC-SA 3.0 license.
Please see the included documents for further information.
*/

#include "UTFT_SdRaw.h"
#include <SdFat.h>
SdFile dataFile;

UTFT_SdRaw::UTFT_SdRaw(UTFT *ptrUTFT)
{
  _UTFT = ptrUTFT;
}

int UTFT_SdRaw::load(int x, int y, int sx, int sy, char *filename, int bufmult, bool iswap)
{
  char buf[2 * bufmult * sx];
  int cx, cy, cp;
  word result;
  if (dataFile.open(filename))
  {
    cbi(_UTFT->P_CS, _UTFT->B_CS);
    cx = 0;
    cy = 0;
    result = bufmult * sx;
    if (_UTFT->orient == PORTRAIT)
    {
      _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
    }
    for (int n = 0; n < sy; n += bufmult)
    {
      result = dataFile.read(&buf, (2 * bufmult) * sx);
      if (_UTFT->orient == PORTRAIT)
      {
        for (int i = 0; i < result; i += 2)
        {
          if (iswap == 1)
          {
            _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
          }
          else
          {
            _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
          }
        }
      }
      else
      {
        cp = 0;
        while (cp < result)
        {
          if (((result - cp) / 2) < (sx - cx))
          {
            _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
            for (int i = (result - cp) - 2; i >= 0; i -= 2)
            {
              if (iswap == 1)
              {
                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
              }
            }
            cx += ((result - cp) / 2);
            cp = result;
          }
          else
          {
            _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
            for (int i = sx - cx - 1; i >= 0; i--)
            {
              if (iswap)
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
              }
            }
            cp += (sx - cx) * 2;
            cx = 0;
            cy++;
          }
        }
      }
    }
    dataFile.close();
    _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
    sbi(_UTFT->P_CS, _UTFT->B_CS);
    return 0;
  }
  else
  {
    return 99;
  }
}

#if defined(__AVR__)
int UTFT_SdRaw::loadS(int x, int y, int sx, int sy, int bufmult, bool iswap)
{
  while (!Serial.find("IMAGE")); // Wait for IMAGE command from host
  byte buf[2 * bufmult * sx];
  int cx, cy, cp;
  word result;
  while(!Serial.available());
  if (Serial.available() > 0)
  {
    cbi(_UTFT->P_CS, _UTFT->B_CS);
    cx = 0;
    cy = 0;
    result = bufmult * sx;
    if (_UTFT->orient == PORTRAIT)
    {
      _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
    }
    for (int n = 0; n < sy; n += bufmult)
    {
      result = Serial.readBytes(buf, ((2 * bufmult) * sx));
      if (_UTFT->orient == PORTRAIT)
      {
        for (int i = 0; i < result; i += 2)
        {
          if (iswap == 1)
          {
            _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
          }
          else
          {
            _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
          }
        }
      }
      else
      {
        cp = 0;
        while (cp < result)
        {
          if (((result - cp) / 2) < (sx - cx))
          {
            _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
            for (int i = (result - cp) - 2; i >= 0; i -= 2)
            {
              if (iswap == 1)
              {
                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
              }
            }
            cx += ((result - cp) / 2);
            cp = result;
          }
          else
          {
            _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
            for (int i = sx - cx - 1; i >= 0; i--)
            {
              if (iswap)
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
              }
            }
            cp += (sx - cx) * 2;
            cx = 0;
            cy++;
          }
        }
      }
    }
    _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
    sbi(_UTFT->P_CS, _UTFT->B_CS);
    return 0;
  }
}
#elif defined(__arm__)
int UTFT_SdRaw::loadS(UARTClass port, int x, int y, int sx, int sy, int bufmult, bool iswap)
{
  while (!port.find("IMAGE")); // Wait for IMAGE command from host
  byte buf[2 * bufmult * sx];
  int cx, cy, cp;
  word result;
  while ((cy = port.available()) == 0);
  if (cy = (port.available()) > 0)
  {
    cbi(_UTFT->P_CS, _UTFT->B_CS);
    cx = 0;
    cy = 0;
    result = bufmult * sx;
    if (_UTFT->orient == PORTRAIT)
    {
      _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
    }
    for (int n = 0; n < sy; n += bufmult)
    {
      result = port.readBytes(buf, ((2 * bufmult) * sx));
      if (_UTFT->orient == PORTRAIT)
      {
        for (int i = 0; i < result; i += 2)
        {
          if (iswap == 1)
          {
            _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
          }
          else
          {
            _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
          }
        }
      }
      else
      {
        cp = 0;
        while (cp < result)
        {
          if (((result - cp) / 2) < (sx - cx))
          {
            _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
            for (int i = (result - cp) - 2; i >= 0; i -= 2)
            {
              if (iswap == 1)
              {
                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
              }
            }
            cx += ((result - cp) / 2);
            cp = result;
          }
          else
          {
            _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
            for (int i = sx - cx - 1; i >= 0; i--)
            {
              if (iswap)
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
              }
            }
            cp += (sx - cx) * 2;
            cx = 0;
            cy++;
          }
        }
      }
    }
    _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
    sbi(_UTFT->P_CS, _UTFT->B_CS);
    return 0;
  }
  else
  {
    return 99;
  }
}

int UTFT_SdRaw::loadS(Serial_ port, int x, int y, int sx, int sy, int bufmult, bool iswap)
{
  while (!port.find("IMAGE")); // Wait for IMAGE command from host
  byte buf[2 * bufmult * sx];
  int cx, cy, cp;
  word result;
  while ((cy = port.available()) == 0);
  if (cy = (port.available()) > 0)
  {
    cbi(_UTFT->P_CS, _UTFT->B_CS);
    cx = 0;
    cy = 0;
    result = bufmult * sx;
    if (_UTFT->orient == PORTRAIT)
    {
      _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
    }
    for (int n = 0; n < sy; n += bufmult)
    {
      result = port.readBytes(buf, ((2 * bufmult) * sx));
      if (_UTFT->orient == PORTRAIT)
      {
        for (int i = 0; i < result; i += 2)
        {
          if (iswap == 1)
          {
            _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
          }
          else
          {
            _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
          }
        }
      }
      else
      {
        cp = 0;
        while (cp < result)
        {
          if (((result - cp) / 2) < (sx - cx))
          {
            _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
            for (int i = (result - cp) - 2; i >= 0; i -= 2)
            {
              if (iswap == 1)
              {
                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
              }
            }
            cx += ((result - cp) / 2);
            cp = result;
          }
          else
          {
            _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
            for (int i = sx - cx - 1; i >= 0; i--)
            {
              if (iswap)
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
              }
            }
            cp += (sx - cx) * 2;
            cx = 0;
            cy++;
          }
        }
      }
    }
    _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
    sbi(_UTFT->P_CS, _UTFT->B_CS);
    return 0;
  }
  else
  {
    return 99;
  }
}
#endif

int UTFT_SdRaw::loadcpld(int x, int y, int sx, int sy, char *filename, int writepage, int bufmult, bool iswap)
{
  char buf[(sx * bufmult * 2)];
  _UTFT->setWritePage(writepage);
  int cx, cy, cp;
  word result;
  if (dataFile.open(filename))
  {
    cbi(_UTFT->P_CS, _UTFT->B_CS);
    cx = 0;
    cy = 0;
    result = bufmult * sx;
    if (_UTFT->orient == PORTRAIT)
    {
      _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
    }
    for (int n = 0; n < sy; n += bufmult)
    {
      result = dataFile.read(&buf, (sx * 2 * bufmult));
      if (_UTFT->orient == PORTRAIT)
      {
        for (int i = 0; i < result; i += 2)
        {
          if (iswap)
          {
            _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
          }
          else
          {
            _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
          }
        }
      }
      else
      {
        cp = 0;
        while (cp < result)
        {
          if (((result - cp) / 2) < (sx - cx))
          {
            _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
            for (int i = (result - cp) - 2; i >= 0; i -= 2)
            {
              if (iswap)
              {
                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
              }
            }
            cx += ((result - cp) / 2);
            cp = result;
          }
          else
          {
            _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
            for (int i = sx - cx - 1; i >= 0; i--)
            {
              if (iswap)
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
              }
            }
            cp += (sx - cx) * 2;
            cx = 0;
            cy++;
          }
        }
      }
    }
    dataFile.close();
    _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
    sbi(_UTFT->P_CS, _UTFT->B_CS);
    return 0;
  }
  else
  {
    return 99;
  }
}


int UTFT_SdRaw::pan(int dispx, int dispy, int sx, int sy, unsigned long offsetx, unsigned long offsety, unsigned long sizex, unsigned long sizey, char *filename, bool iswap)
{
  char buffer[2 * sx];
  int cx, cy, cp;
  word temp, result;
  unsigned long temp1, temp2;
  temp1 = sizex * 2;
  temp2 = offsetx * 2;
  if (dataFile.open(filename))
  {
    cbi(_UTFT->P_CS, _UTFT->B_CS);
    cx = 0;
    cy = 0;
    result = sx;
    if (_UTFT->orient == PORTRAIT)
    {
      _UTFT->setXY(dispx, dispy, dispx + sx - 1, dispy + sy - 1);
    }
    for (int n = 0; n < sy; n++)
    {
      dataFile.seekSet(((n + offsety)*temp1) + temp2);
      result = dataFile.read(&buffer, 2 * sx);
      if (_UTFT->orient == PORTRAIT)
      {
        for (int i = 0; i < result; i += 2)
        {
          if (iswap == 1)
          {
            _UTFT->LCD_Write_DATA(buffer[i + 1], buffer[i]);
          }
          else
          {
            _UTFT->LCD_Write_DATA(buffer[i], buffer[i + 1]);
          }
        }
      }
      else
      {
        cp = 0;
        while (cp < result)
        {
          if (((result - cp) / 2) < (sx - cx))
          {
            _UTFT->setXY(dispx + cx, dispy + cy, dispx + cx + ((result - cp) / 2) - 1, dispy + cy);
            for (int i = (result - cp) - 2; i >= 0; i -= 2)
            {
              if (iswap == 1)
              {
                _UTFT->LCD_Write_DATA(buffer[cp + i + 1], buffer[cp + i]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buffer[cp + i], buffer[cp + i + 1]);
              }
            }
            cx += ((result - cp) / 2);
            cp = result;
          }
          else
          {
            _UTFT->setXY(dispx + cx, dispy + cy, dispx + sx - 1, dispy + cy);
            for (int i = sx - cx - 1; i >= 0; i--)
            {
              if (iswap == 1)
              {
                _UTFT->LCD_Write_DATA(buffer[cp + (i * 2) + 1], buffer[cp + (i * 2)]);
              }
              else
              {
                _UTFT->LCD_Write_DATA(buffer[cp + (i * 2)], buffer[cp + (i * 2) + 1]);
              }
            }
            cp += (sx - cx) * 2;
            cx = 0;
            cy++;
          }
        }
      }
    }
    dataFile.close();
    _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
    sbi(_UTFT->P_CS, _UTFT->B_CS);
    return 0;
  }
  else
  {
    return 99;
  }
}

/* int UTFT_SdRaw::panscale(int x, int y, int sx, int sy, unsigned long offsetx, unsigned long offsety, unsigned long sizex, unsigned long sizey, char *filename, byte scale)
{
  char buffer[sx*2*scale];
  int cx, cy, cp;
  word result;
  unsigned long temp1,temp2;
  temp1=sizex*2;
  temp2=offsetx*2;
  if (dataFile.open(filename))
  {
    cbi(_UTFT->P_CS, _UTFT->B_CS);
    cx=0;
    cy=0;
    result=2*sx*scale;
    if (_UTFT->orient==PORTRAIT)
    {
      _UTFT->setXY(x, y, x+sx-1, y+sy-1);
    }
    for(int n=0;n<sy;n++)
    {
      dataFile.seekSet(((n*scale+offsety)*temp1)+temp2);
      result=dataFile.read(&buffer,2*sx*scale);
      if (result==2*sx*scale)
      {
        if (_UTFT->orient==PORTRAIT)
        {
          for (int i=0; i<result; i+=(2*scale))
            _UTFT->LCD_Write_DATA(buffer[i+1],buffer[i]);
        }
        else
        {
          cp=0;
          while (cp<result)
          {
            if (((result-cp)/2)<(sx-cx))
            {
              _UTFT->setXY(x+cx, y+cy, x+cx+((result-cp)/2)-1, y+cy);
              for (int i=(result-cp)-2; i>=0; i-=2)
                _UTFT->LCD_Write_DATA(buffer[cp+i+1],buffer[cp+i]);
              cx+=((result-cp)/2);
              cp=result;
            }
            else
            {
              _UTFT->setXY(x+cx, y+cy, x+sx-1, y+cy);
              for (int i=sx-cx-1; i>=0; i--)
                _UTFT->LCD_Write_DATA(buffer[cp+(i*2)+1],buffer[cp+(i*2)]);
              cp+=(sx-cx)*2;
              cx=0;
              cy++;
            }
          }
        }
      }
      else
      {
        dataFile.close();
        _UTFT->setXY(0,0,_UTFT->getDisplayXSize()-1,_UTFT->getDisplayYSize()-1);
        sbi(_UTFT->P_CS, _UTFT->B_CS);
        return 99;
      }
    }
    dataFile.close();
    _UTFT->setXY(0,0,_UTFT->getDisplayXSize()-1,_UTFT->getDisplayYSize()-1);
    sbi(_UTFT->P_CS, _UTFT->B_CS);
    return 0;
  }
  else
  {
    return 99;
  }
}
 */