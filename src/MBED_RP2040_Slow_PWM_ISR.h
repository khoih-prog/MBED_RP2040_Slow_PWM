/****************************************************************************************************************************
  MBED_RP2040_Slow_ISR.h
  For MBED RP2040-based boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/MBED_RP2040_Slow_PWM
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one MBED RP2040-based timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.2.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      22/09/2021 Initial coding for RP2040-based boards using Arduino-mbed mbed_nano or mbed_rp2040 core
  1.0.1   K Hoang      22/10/2021 Fix platform in library.json for PIO
  1.1.0   K Hoang      10/11/2021 Add functions to modify PWM settings on-the-fly
  1.2.0   K Hoang      02/02/2022 Fix multiple-definitions linker error. Improve accuracy. Optimize code
*****************************************************************************************************************************/

#pragma once

#ifndef MBED_RP2040_SLOW_PWM_ISR_H
#define MBED_RP2040_SLOW_PWM_ISR_H

#include "MBED_RP2040_Slow_PWM_ISR.hpp"
#include "MBED_RP2040_Slow_PWM_ISR_Impl.h"

#endif    // MBED_RP2040_SLOW_PWM_ISR_H


