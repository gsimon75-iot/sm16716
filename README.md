# SM16716 LED driver library for Arduino AVR and ESP8266

Arduino library for the Shenzhen Sunmoon SM16716 and SM16726 LED drivers.

[![version](https://img.shields.io/badge/version-1.0.0-brightgreen.svg)](CHANGELOG.md)
[![travis](https://travis-ci.org/gsimon75/sm16716.svg?branch=master)](https://travis-ci.org/gsimon75/sm16716)

Inspired by the [MY92xx](https://github.com/xoseperez/my92xx.git) module by Xose Pérez.


## Example

A simple example, turning the blue LEDs on.

```
#include <sm16716.h>

#define SM16716_CHIPS    1                      // No daisy-chain
#define SM16716_CLK_PIN  4                      // CLK GPIO
#define SM16716_DAT_PIN 14                      // DAT GPIO

sm16716 _sm16716 = sm16716(SM16716_CHIPS, SM16716_CLK_PIN, SM16716_DAT_PIN);

void setup() {
    _sm16716.setChannel(0, 2, 255); // unit 0 is the 1st controller, channel 2 is blue, 255 is the maximal intensity
    _sm16716.update();
}

void loop() {}

```
