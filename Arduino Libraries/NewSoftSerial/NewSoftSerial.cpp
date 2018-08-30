/*
NewSoftSerial.h - Multi-instance software serial library
Copyright (c) 2006 David A. Mellis.  All rights reserved.
Interrupt-driven receive and other improvements by ladyada 
Bug fixes, circular buffer, Print derivation, and multi-
instance support by mikalhart

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

/******************************************************************************
* Includes
******************************************************************************/
#include <avr/interrupt.h>
#include "WConstants.h"
#include "NewSoftSerial.h"

/******************************************************************************
* Statics
******************************************************************************/
NewSoftSerial *NewSoftSerial::active_object = 0;
char NewSoftSerial::_receive_buffer[_NewSS_MAX_RX_BUFF]; 
volatile uint8_t NewSoftSerial::_receive_buffer_tail = 0;
volatile uint8_t NewSoftSerial::_receive_buffer_head = 0;

/******************************************************************************
* Private methods
******************************************************************************/
#if (F_CPU == 16000000 || F_CPU == 8000000)
/* static */ 
void NewSoftSerial::tunedDelay(uint16_t delay) { 
  uint8_t tmp=0;

  asm volatile("sbiw    %0, 0x01 \n\t"
    "ldi %1, 0xFF \n\t"
    "cpi %A0, 0xFF \n\t"
    "cpc %B0, %1 \n\t"
    "brne .-10 \n\t"
    : "+r" (delay), "+a" (tmp)
    : "0" (delay)
    );
}
#endif

// This function sets the current object as the "active"
// one and returns true if it replaces another 
bool NewSoftSerial::activate(void)
{
  if (active_object != this)
  {
    _buffer_overflow = false;
    uint8_t oldSREG = SREG;
    cli();
    _receive_buffer_head = _receive_buffer_tail = 0;
    active_object = this;
    SREG = oldSREG;
    return true;
  }

  return false;
}

// The receive routine called by the interrupt handler
void NewSoftSerial::recv()
{
// Work-around for avr-gcc 4.3 OSX version bug
// Preserve the registers that the compiler misses
// (courtesy of Arduino forum user *etracer*)

  asm volatile(
	"push r18 \n\t"
	"push r19 \n\t"
	"push r20 \n\t"
	"push r21 \n\t"
	"push r22 \n\t"
	"push r23 \n\t"
	"push r26 \n\t"
	"push r27 \n\t"
	::);

  char i, d = 0;

  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (!digitalRead(_receivePin)) 
  {
    // Wait approximately 1/2 of a bit width to "center" the sample
    tunedDelay(_bitDelay / 2 - 8);

    // Read each of the 8 bits
    for (i=0; i<8; i++) 
    {
      //PORTB |= _BV(5);
      tunedDelay(_bitDelay - 6);  // 6 - digitalread takes some time
      //PORTB &= ~_BV(5);
      if (digitalRead(_receivePin))
        d |= (1 << i);
      else // else clause added to ensure function timing is ~balanced
        d &= ~(1 << i);
    }

    // skip the stop bit
    tunedDelay(_bitDelay);

    // if buffer full, set the overflow flag and return
    if ((_receive_buffer_tail + 1) % _NewSS_MAX_RX_BUFF != _receive_buffer_head) 
    {
      // save new data in buffer: tail points to where byte goes
      _receive_buffer[_receive_buffer_tail] = d; // save new byte
      _receive_buffer_tail = (_receive_buffer_tail + 1) % _NewSS_MAX_RX_BUFF;
    } 
    else 
    {
      _buffer_overflow = true;
    }
  }

// Work-around for avr-gcc 4.3 OSX version bug
// Restore the registers that the compiler misses
  asm volatile(
	"pop r27 \n\t"
	"pop r26 \n\t"
	"pop r23 \n\t"
	"pop r22 \n\t"
	"pop r21 \n\t"
	"pop r20 \n\t"
	"pop r19 \n\t"
	"pop r18 \n\t"
	::);
}
 

/******************************************************************************
* Interrupt handling
******************************************************************************/

/* static */
inline void NewSoftSerial::handle_interrupts(int lopin, int hipin)
{
  if (active_object && 
    active_object->_receivePin >= lopin && 
    active_object->_receivePin <= hipin)
  {
    active_object->recv();
  }
}

SIGNAL(PCINT0_vect) 
{
  NewSoftSerial::handle_interrupts(8, 13);
}

SIGNAL(PCINT1_vect) 
{
  NewSoftSerial::handle_interrupts(14, 21);
}

SIGNAL(PCINT2_vect)
{
  NewSoftSerial::handle_interrupts(0, 7);
}

/******************************************************************************
* Constructor
******************************************************************************/
NewSoftSerial::NewSoftSerial(uint8_t receivePin, uint8_t transmitPin) : 
  _bitDelay(0),
  _txDelta(0),
  _buffer_overflow(false)
{
  setTX(transmitPin);
  setRX(receivePin);
}

/******************************************************************************
* Public methods
******************************************************************************/
void NewSoftSerial::setTX(uint8_t tx)
{
  _transmitPin = tx;
  pinMode(_transmitPin, OUTPUT);
  digitalWrite(_transmitPin, HIGH);
}

void NewSoftSerial::setRX(uint8_t rx)
{
  _receivePin = rx;
  pinMode(_receivePin, INPUT); 
  digitalWrite(_receivePin, HIGH);  // pullup!
}

void NewSoftSerial::begin(long speed)
{
  _txDelta = 0;
  switch (speed) 
  {
  case 115200: // For xmit -only-!
    _bitDelay = 8; break;
  case 57600:
    _txDelta = -3;
    _bitDelay = 31; break;
  case 38400:
    _txDelta = -3;
    _bitDelay = 51; break; 
  case 31250:
    _bitDelay = 62; break;
  case 28800:
    _txDelta = -4;
    _bitDelay = 70; break;
  case 19200:
    _bitDelay = 108; break;
  case 14400:
    _bitDelay = 152; break;
  case 9600:
    _bitDelay = 226; break;
  case 4800:
    _bitDelay = 464; break;
  case 2400:
    _bitDelay = 940; break;
  case 1200:
    _bitDelay = 1868; break;
  case 300:
    _bitDelay = 7436; break;
  default:
    _bitDelay = 0;
  }

  // Set up RX interrupts, but only if we have a valid RX baud rate
  if (_bitDelay && speed <= 57600)
  {
    if (_receivePin < 8) {
      // a PIND pin, PCINT16-23
      PCICR |= _BV(2);
      PCMSK2 |= _BV(_receivePin);
    } else if (_receivePin <= 13) {
      // a PINB pin, PCINT0-7
      PCICR |= _BV(0);    
      PCMSK0 |= _BV(_receivePin-8);
    } else if (_receivePin <= 21) {
      // a PINC pin, PCINT8-14/15
      PCICR |= _BV(1);
      PCMSK1 |= _BV(_receivePin-14);
    } 

#if (F_CPU == 8000000)
    if (_bitDelay)
      _bitDelay = _bitDelay / 2 + 6;
#endif
    tunedDelay(_bitDelay); // if we were low this establishes the end
  }

  activate();
}

// Read data from buffer
int NewSoftSerial::read(void)
{
  uint8_t d;

  // A newly activated object never has any rx data
  if (activate())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _NewSS_MAX_RX_BUFF;
  return d;
}

uint8_t NewSoftSerial::available(void)
{
  // A newly activated object never has any rx data
  if (activate())
    return 0;

  return (_receive_buffer_tail + _NewSS_MAX_RX_BUFF - _receive_buffer_head) % _NewSS_MAX_RX_BUFF;
}

void NewSoftSerial::write(uint8_t b)
{
  if (_bitDelay == 0)
    return;

  activate();

  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit

  // Write the start bit
  digitalWrite(_transmitPin, LOW);
  tunedDelay(_bitDelay + _txDelta);

  // Write each of the 8 bits
  for (byte mask = 0x01; mask; mask <<= 1) {
    if (b & mask){ // choose bit
      digitalWrite(_transmitPin, HIGH); // send 1
    }
    else{
      digitalWrite(_transmitPin, LOW); // send 0
    }
    tunedDelay(_bitDelay + _txDelta);
  }

  digitalWrite(_transmitPin, HIGH);
  SREG = oldSREG; // turn interrupts back on. hooray!
  tunedDelay(_bitDelay + _txDelta);
}
