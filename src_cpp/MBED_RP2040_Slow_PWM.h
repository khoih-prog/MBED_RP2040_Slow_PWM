/****************************************************************************************************************************
  MBED_RP2040_Slow_PWM.h
  For MBED RP2040-based boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/MBED_RP2040_Slow_PWM
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one MBED RP2040-based timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.0.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      22/09/2021 Initial coding for RP2040-based boards using Arduino-mbed mbed_nano or mbed_rp2040 core
  1.0.1   K Hoang      22/10/2021 Fix platform in library.json for PIO
*****************************************************************************************************************************/

#pragma once

#ifndef MBED_RP2040_SLOW_PWM_H
#define MBED_RP2040_SLOW_PWM_H

#if ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
      defined(ARDUINO_GENERIC_RP2040) ) && defined(ARDUINO_ARCH_MBED)
  #warning Using MBED RASPBERRY_PI_PICO platform
#else
  #error This code is intended to run on the MBED RASPBERRY_PI_PICO platform! Please check your Tools->Board setting.
#endif

#ifndef MBED_RP2040_SLOW_PWM_VERSION
  #define MBED_RP2040_SLOW_PWM_VERSION       "MBED_RP2040_Slow_PWM v1.0.1"
#endif

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_      1
#endif

#include "PWM_Generic_Debug.h"

/*
  To enable an alarm:
  • Enable the interrupt at the timer with a write to the appropriate alarm bit in INTE: i.e. (1 << 0) for ALARM0
  • Enable the appropriate timer interrupt at the processor (see Section 2.3.2)
  • Write the time you would like the interrupt to fire to ALARM0 (i.e. the current value in TIMERAWL plus your desired
    alarm time in microseconds). Writing the time to the ALARM register sets the ARMED bit as a side effect.
  Once the alarm has fired, the ARMED bit will be set to 0 . To clear the latched interrupt, write a 1 to the appropriate bit in
  INTR.
  
  Defined in x.y.z/cores/arduino/mbed/targets/TARGET_RASPBERRYPI/TARGET_RP2040/pico-sdk/rp2_common/hardware_timer/include/hardware/timer.h
  
  typedef void (*hardware_alarm_callback_t)(uint alarm_num);
  
  void hardware_alarm_set_callback(uint alarm_num, hardware_alarm_callback_t callback);
  bool hardware_alarm_set_target(uint alarm_num, absolute_time_t t);  
  void hardware_alarm_cancel(uint alarm_num);
*/

// We can use many timers here
#define MAX_RPI_PICO_NUM_TIMERS      4

absolute_time_t absAlarmTime[MAX_RPI_PICO_NUM_TIMERS]; 
volatile uint64_t        _timerCount [MAX_RPI_PICO_NUM_TIMERS];

void TIMER_ISR_START(uint alarm_num)
{  
  absAlarmTime[alarm_num]._private_us_since_boot = time_us_64() + _timerCount[alarm_num];                
  hardware_alarm_set_target(alarm_num, absAlarmTime[alarm_num]);
}

void TIMER_ISR_END(uint alarm_num)
{
}

class MBED_RP2040_TimerInterrupt;

typedef MBED_RP2040_TimerInterrupt MBED_RP2040_Timer;
   
class MBED_RP2040_TimerInterrupt
{
  private:
   
    uint8_t                       _timerNo;
    hardware_alarm_callback_t     _callback;          // pointer to the local callback function
    float                         _frequency;         // Timer frequency
        
  public:

    MBED_RP2040_TimerInterrupt(uint8_t timerNo)
    {     
      _timerNo = timerNo;
      _callback = NULL;
    };

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here
    bool setFrequency(float frequency, hardware_alarm_callback_t callback)
    {
      if (_timerNo < MAX_RPI_PICO_NUM_TIMERS)
      {             
        // Hardware timer is preset in RP2040 at 1MHz / 1uS
        _frequency  = (float) 1000000;
        _timerCount[_timerNo] = (uint64_t) _frequency / frequency;
        
        PWM_LOGWARN3(F("MBED_RP2040_TimerInterrupt: _timerNo ="), _timerNo, F(", _fre ="), _frequency);
        PWM_LOGWARN3(F("_count ="), (uint32_t) (_timerCount[_timerNo] >> 32) , F("-"), (uint32_t) (_timerCount[_timerNo]));
        
        _callback  =  callback;
         
        //void hardware_alarm_set_callback(uint alarm_num, hardware_alarm_callback_t callback);
        //param callback the callback to install, or NULL to unset
        hardware_alarm_set_callback(_timerNo, callback);       
        
        TIMER_ISR_START(_timerNo);
               
        //bool hardware_alarm_set_target(uint alarm_num, absolute_time_t t);
        hardware_alarm_set_target(_timerNo, absAlarmTime[_timerNo]);
         
        PWM_LOGWARN1(F("hardware_alarm_set_target, uS ="), _timerCount[_timerNo]);

        return true;
      }
      else
      {
        PWM_LOGERROR(F("Error. Timer must be 0-3"));
        
        return false;
      }
      
      TIMER_ISR_END(_timerNo);
    }

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here
    bool setInterval(unsigned long interval, hardware_alarm_callback_t callback)
    {
      return setFrequency((float) (1000000.0f / interval), callback);
    }

    bool attachInterrupt(float frequency, hardware_alarm_callback_t callback)
    {
      return setFrequency(frequency, callback);
    }

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be added in the future by adding similar functions here
    bool attachInterruptInterval(unsigned long interval, hardware_alarm_callback_t callback)
    {
      return setFrequency( (float) ( 1000000.0f / interval), callback);
    }

    void detachInterrupt()
    {
      hardware_alarm_set_callback(_timerNo, NULL);
    }

    void disableTimer()
    {
      hardware_alarm_set_callback(_timerNo, NULL);
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt()
    {
      TIMER_ISR_START(_timerNo);
      hardware_alarm_set_callback(_timerNo, _callback);
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer()
    {
      TIMER_ISR_START(_timerNo);
      hardware_alarm_set_callback(_timerNo, _callback);
    }

    // Just stop clock source, clear the count
    void stopTimer()
    {
      hardware_alarm_set_callback(_timerNo, NULL);
    }

    // Just reconnect clock source, start current count from 0
    void restartTimer()
    {
      TIMER_ISR_START(_timerNo);
      hardware_alarm_set_callback(_timerNo, _callback);
    }

    int8_t getTimer() __attribute__((always_inline))
    {
      return _timerNo;
    }
}; // class MBED_RP2040_TimerInterrupt

#include "MBED_RP2040_Slow_PWM_ISR.h"

#endif    // MBED_RP2040_SLOW_PWM_H

