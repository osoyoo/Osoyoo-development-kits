/*
NewSoftSerial.h - Multi-instance software serial library for Arduino
Copyright (c) 2006 David A. Mellis.  All rights reserved.
Interrupt-driven receive and other improvements by ladyada 
Bug fixes, circular buffer, "Print" class derivation, and 
multi-instance support by mikalhart.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef NewSoftSerial_h
#define NewSoftSerial_h

#include <inttypes.h>
#include "Print.h"

/******************************************************************************
* Definitions
******************************************************************************/

#define _NewSS_MAX_RX_BUFF 64 // RX buffer size
#define _NewSS_VERSION	5 // software version of this library

class NewSoftSerial : public Print
{
private:
  // per object data
  uint8_t _receivePin;
  uint8_t _transmitPin;
  uint16_t _bitDelay;
  signed char _txDelta; // a "fudge" factor for bitDelay
  bool _buffer_overflow;

  // static data
  static char _receive_buffer[_NewSS_MAX_RX_BUFF]; 
  static volatile uint8_t _receive_buffer_tail;
  static volatile uint8_t _receive_buffer_head;
  static NewSoftSerial *active_object;

  // private methods
  void recv();
  bool activate();
  virtual void write(uint8_t byte);

  // private static method for timing
  static void tunedDelay(uint16_t delay);

public:
  // public methods
  NewSoftSerial(uint8_t receivePin, uint8_t transmitPin);
  void setTX(uint8_t transmitPin);
  void setRX(uint8_t receivePin);
  void begin(long speed);
  int read();
  uint8_t available(void);
  bool active() { return this == active_object; }
  bool overflow() { bool ret = _buffer_overflow; _buffer_overflow = false; return ret; }
  static int library_version() { return _NewSS_VERSION; }

  // public only for easy access by interrupt handlers
  static inline void handle_interrupts(int lopin, int hipin);
};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round 

#endif
