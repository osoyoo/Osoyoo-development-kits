/*
UTFT_SdRaw.cpp - Add-on library to integrate UTFT and SdFat.
Copyright (C)2014 Graham Lawrence. Basic functionality of this library
is based on UTFT_tinyFAT by Henning Karlsen. All rights reserved

This library is free software; you can redistribute it and/or
modify it under the terms of the CC BY-NC-SA 3.0 license.
Please see the included documents for further information.
*/


#ifndef UTFT_SdRaw_h
#define UTFT_SdRaw_h
#include <UTFT.h>
#if ((!defined(UTFT_VERSION)) || (UTFT_VERSION<241))
#error : You will need UTFT v2.41 or higher to use this add-on library...
#endif


class UTFT_SdRaw
{
  public:
    UTFT_SdRaw(UTFT *ptrUTFT);

    int load(int x, int y, int sx, int sy, char *filename, int bufmult = 1, bool iswap = 0);
#if defined(__AVR__)
    int loadS(int x, int y, int sx, int sy, int bufmult, bool iswap);
#elif defined(__arm__)
    int loadS(Serial_ port, int x, int y, int sx, int sy, int bufmult, bool iswap);
    int loadS(UARTClass port, int x, int y, int sx, int sy, int bufmult, bool iswap);
#endif
    int loadcpld(int x, int y, int sx, int sy, char *filename, int writepage = 0, int bufmult = 1, bool iswap = 0);
    int pan(int x, int y, int sx, int sy, unsigned long offsetx, unsigned long offsety, unsigned long sizex, unsigned long sizey, char *filename, bool iswap = 0);
    // int panscale(int x, int y, int sx, int sy, unsigned long offsetx, unsigned long offsety, unsigned long sizex, unsigned long sizey, char *filename, byte scale);

  protected:
    UTFT *_UTFT;
};

#endif