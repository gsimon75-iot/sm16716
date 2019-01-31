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

#ifndef _sm16716_h
#define _sm16716_h

#include <Arduino.h>

#ifdef DEBUG_SM16716
    #if ARDUINO_ARCH_ESP8266
        #define DEBUG_MSG_SM16716(...) DEBUG_SM16716.printf( __VA_ARGS__ )
    #elif ARDUINO_ARCH_AVR
        #define DEBUG_MSG_SM16716(...) { char buffer[80]; snprintf(buffer, sizeof(buffer),  __VA_ARGS__ ); DEBUG_SM16716.print(buffer); }
    #endif
#else
    #define DEBUG_MSG_SM16716(...)
#endif

class sm16716 {

    public:

        sm16716(uint8_t chips, uint8_t clk, uint8_t dat);
        void setChannel(uint8_t chip, uint8_t channel, unsigned int value);
        unsigned int getChannel(uint8_t chip, uint8_t channel);
        void update();

    private:

        void _send_bit(uint8_t v);
        void _send_byte(uint8_t v);

        uint8_t _chips;
        uint8_t * _value;
        uint8_t _pin_clk;
        uint8_t _pin_dat;

};

#endif
