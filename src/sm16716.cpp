/*
SM16716 LED Driver Arduino library 1.0.0

SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 
Copyright (C) 2019 Gabor Simon. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

#include "sm16716.h"

#if ARDUINO_ARCH_ESP8266

    extern "C" {
        void os_delay_us(unsigned int);
    }

#elif ARDUINO_ARCH_AVR

    #define os_delay_us delayMicroseconds

#endif

void sm16716::_send_bit(uint8_t v) {
    /* NOTE:
     * According to the spec sheet, max freq is 30 MHz, that is 16.6 ns per high/low half of the
     * clk square wave. That is less than the overhead of 'digitalWrite' at this clock rate,
     * so no additional delays are needed yet. */

    digitalWrite(_pin_dat, (v != 0) ? HIGH : LOW);
    //delayMicroseconds(1);
    digitalWrite(_pin_clk, HIGH);
    //delayMicroseconds(1);
    digitalWrite(_pin_clk, LOW);
}

void sm16716::_send_byte(uint8_t v) {
    for (uint8_t mask = 0x80; mask; mask >>= 1) {
        _send_bit(v & mask);
    }
}

// -----------------------------------------------------------------------------

void sm16716::setChannel(uint8_t chip, uint8_t channel, unsigned int value) {
    DEBUG_MSG_SM16716("[SM16716] setChannel(%d, %d, %d)\n", chip, channel, value);
    if (chip < _chips && channel < 3) {
        _value[chip * 3 + channel] = value;
    }
}

unsigned int sm16716::getChannel(uint8_t chip, uint8_t channel) {
    if (chip < _chips && channel < 3) {
        return _value[chip * 3 + channel];
    }
    return 0;
}

void sm16716::update() {
  uint8_t const * data = _value;
  DEBUG_MSG_SM16716("[SM16716] update\n");
  for (uint8_t chip = 0; chip < _chips; ++chip) {
      // send start bit
      _send_bit(1);
      // send 24-bit rgb data
      _send_byte(data[0]);
      _send_byte(data[1]);
      _send_byte(data[2]);
      DEBUG_MSG_SM16716("[SM16716] Sent #%02x%02x%02x\n", data[0], data[1], data[2]);
      data += 3;
  }
  // send a 'do it' pulse
  _send_bit(0);
  _send_byte(0);
  _send_byte(0);
  _send_byte(0);

}

// -----------------------------------------------------------------------------

sm16716::sm16716(uint8_t chips, uint8_t clk, uint8_t dat) {

    _chips = chips;
	_pin_clk = clk;
	_pin_dat = dat;

    _value = new uint8_t[_chips * 3];
    for (uint8_t i=0; i<_chips * 3; i++) {
        _value[i] = 0;
    }

    // Init GPIO
	pinMode(_pin_clk, OUTPUT);
	digitalWrite(_pin_clk, LOW);
	pinMode(_pin_dat, OUTPUT);
	digitalWrite(_pin_dat, LOW);

	// Reset previous operation (if any)
    for (uint8_t i = 0; i < 50; ++i) {
        _send_bit(0);
    }

    DEBUG_MSG_SM16716("[SM16716] Initialized\n");

}
