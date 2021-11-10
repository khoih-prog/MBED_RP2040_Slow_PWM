# MBED_RP2040_Slow_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/MBED_RP2040_Slow_PWM.svg?)](https://www.ardu-badge.com/MBED_RP2040_Slow_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/MBED_RP2040_Slow_PWM.svg)](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/MBED_RP2040_Slow_PWM.svg)](http://github.com/khoih-prog/MBED_RP2040_Slow_PWM/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.1.0](#releases-v110)
  * [Releases v1.0.1](#Releases-v101)
  * [Initial Releases v1.0.0](#Initial-Releases-v100)

---
---

## Changelog

### Releases v1.1.0

1. Add functions to modify PWM settings on-the-fly
2. Add example to demo how to modify PWM settings on-the-fly

### Releases v1.0.1

1. Fix platform in `library.json`

### Initial Releases v1.0.0

1. Initial coding to support **RP2040-based boards** such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, etc. using RP2040 [**ArduinoCore-mbed mbed_nano or mbed_rp2040** core](https://github.com/arduino/ArduinoCore-mbed)
2. The hybrid ISR-based PWM channels can generate from very low (much less than 1Hz) to highest PWM frequencies up to 1000Hz with acceptable accuracy.

---
---

## Copyright

Copyright 2021- Khoi Hoang


