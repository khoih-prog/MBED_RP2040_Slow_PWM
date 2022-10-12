# MBED_RP2040_Slow_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/MBED_RP2040_Slow_PWM.svg?)](https://www.ardu-badge.com/MBED_RP2040_Slow_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/MBED_RP2040_Slow_PWM.svg)](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/MBED_RP2040_Slow_PWM.svg)](http://github.com/khoih-prog/MBED_RP2040_Slow_PWM/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Important Change from v1.2.0](#Important-Change-from-v120)
* [Why do we need this MBED_RP2040_Slow_PWM library](#why-do-we-need-this-MBED_RP2040_Slow_PWM-library)
  * [Features](#features)
  * [Why using ISR-based PWM is better](#why-using-isr-based-pwm-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [More useful Information](#more-useful-information)
* [Usage](#usage)
  * [1. Init Hardware Timer](#1-init-hardware-timer)
  * [2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions](#2-Set-PWM-Frequency-dutycycle-attach-irqCallbackStartFunc-and-irqCallbackStopFunc-functions)
* [Examples](#examples)
  * [ 1. ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array)
  * [ 2. ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)
  * [ 3. ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple)
  * [ 4. ISR_Changing_PWM](examples/ISR_Changing_PWM)
  * [ 5. ISR_Modify_PWM](examples/ISR_Modify_PWM)
  * [ 6. multiFileProject](examples/multiFileProject). **New**
* [Example ISR_16_PWMs_Array_Complex](#Example-ISR_16_PWMs_Array_Complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_PWMs_Array_Complex on RaspberryPi Pico](#1-ISR_16_PWMs_Array_Complex-on-RaspberryPi-Pico)
  * [2. ISR_16_PWMs_Array on RaspberryPi Pico](#2-ISR_16_PWMs_Array-on-RaspberryPi-Pico)
  * [3. ISR_16_PWMs_Array_Simple on RaspberryPi Pico](#3-ISR_16_PWMs_Array_Simple-on-RaspberryPi-Pico)
  * [4. ISR_Modify_PWM on RaspberryPi Pico](#4-ISR_Modify_PWM-on-RaspberryPi-Pico)
  * [5. ISR_Changing_PWM on RaspberryPi Pico](#5-ISR_Changing_PWM-on-RaspberryPi-Pico)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Change from v1.2.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

As more complex calculation and check **inside ISR** are introduced from v1.2.0, there is possibly some crash depending on use-case.

You can modify to use larger `HW_TIMER_INTERVAL_US`, (from current 20uS), according to your board and use-case if crash happens.


```cpp
// Current 20uS
#define HW_TIMER_INTERVAL_US      20L
```

---
---

### Why do we need this [MBED_RP2040_Slow_PWM library](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM)

### Features

This library enables you to use Hardware Timers on RP2040-based boards to create and output PWM to pins. Because this library doesn't use the powerful hardware-controlled PWM with limitations, the maximum PWM frequency is currently limited at **1000Hz**, which is still suitable for many real-life applications. Now you can also modify PWM settings on-the-fly.

---

This library enables you to use Interrupt from Hardware Timers on RP2040-based boards to create and output PWM to pins. It now supports 16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer. PWM interval can be very long (uint32_t millisecs). The most important feature is they're ISR-based PWM channels. Therefore, their executions are not blocked by bad-behaving functions or tasks. This important feature is absolutely necessary for mission-critical tasks. These hardware PWM channels, using interrupt, still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software PWM using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based PWM-channels**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software PWM channels. 

The most important feature is they're ISR-based PWM channels. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_16_PWMs_Array_Complex**](examples/ISR_16_PWMs_Array_Complex) example will demonstrate the nearly perfect accuracy, compared to software PWM, by printing the actual period / duty-cycle in `microsecs` of each of PWM-channels.

Being ISR-based PWM, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet or Blynk services. You can also have many `(up to 16)` PWM channels to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see `software-based` SimpleTimer is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

#### Why using ISR-based PWM is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware PWM channels, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software PWM channels using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software PWM channels, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. **RP2040-based boards** such as Nano_RP2040_Connect, RaspberryPi Pico, etc., using [**ArduinoCore-mbed mbed_nano or mbed_rp2040** core](https://github.com/arduino/ArduinoCore-mbed)

---

#### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

---
---

## Prerequisites

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
2. [`Arduino mbed_rp2040 core 3.3.0+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino (Use Arduino Board Manager) RP2040-based boards, such as **Arduino Nano RP2040 Connect, RASPBERRY_PI_PICO, etc.**. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest)
3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_Timers_Array_Simple](examples/ISR_Timers_Array_Simple) and [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) examples.
   
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**MBED_RP2040_Slow_PWM**](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/MBED_RP2040_Slow_PWM.svg?)](https://www.ardu-badge.com/MBED_RP2040_Slow_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**MBED_RP2040_Slow_PWM**](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM) page.
2. Download the latest release `MBED_RP2040_Slow_PWM-main.zip`.
3. Extract the zip file to `MBED_RP2040_Slow_PWM-main` directory 
4. Copy whole `MBED_RP2040_Slow_PWM-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**MBED_RP2040_Slow_PWM** library](https://registry.platformio.org/libraries/khoih-prog/MBED_RP2040_Slow_PWM) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/MBED_RP2040_Slow_PWM/installation). Search for **MBED_RP2040_Slow_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```cpp
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "MBED_RP2040_Slow_PWM.hpp"     //https://github.com/khoih-prog/MBED_RP2040_Slow_PWM
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```cpp
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "MBED_RP2040_Slow_PWM.h"           //https://github.com/khoih-prog/MBED_RP2040_Slow_PWM
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

Have a look at the discussion in [Different behaviour using the src_cpp or src_h lib #80](https://github.com/khoih-prog/ESPAsync_WiFiManager/discussions/80)


---
---

## More useful Information

The RP2040-based system timer peripheral provides a `global microsecond timebase` for the system, and generates interrupts based on this timebase. It supports the following features:

  • A single 64-bit counter, incrementing once per microsecond
  • This counter can be read from a pair of latching registers, for race-free reads over a 32-bit bus.
  • Four alarms: match on the lower 32 bits of counter, IRQ on match: **TIMER_IRQ_0-TIMER_IRQ_3**

---

Now with these new `16 ISR-based PWM channels` (while consuming only **1 hardware timer**), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software PWM channels. The most important feature is they're ISR-based PWM channels Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) example will demonstrate the nearly perfect accuracy compared to software PWM channels by printing the actual elapsed microsecs / millisecs of each type of PWM channels.

Being ISR-based PWM channels, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` PWM channels to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 
You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task in loop(), using delay() function as an example. The elapsed time then is very unaccurate


---
---

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.
`TIMER_IRQ_0, TIMER_IRQ_1, TIMER_IRQ_2 and TIMER_IRQ_3` are supported for RP2040-based boards.

#### 1. Init Hardware Timer

```cpp
// Init MBED_RPI_PICO_Timer
MBED_RP2040_Timer ITimer(0);;

// Init MBED_RP2040_Slow_PWM, each can service 16 different ISR-based PWM channels
MBED_RP2040_Slow_PWM ISR_PWM;
```

#### 2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions

```cpp
void irqCallbackStartFunc()
{

}

void irqCallbackStopFunc()
{

}

void setup()
{
  ....
  
  // You can use this with PWM_Freq in Hz
  ISR_PWM.setPWM(PWM_Pin, PWM_Freq, PWM_DutyCycle, irqCallbackStartFunc, irqCallbackStopFunc);
                   
  ....                 
}  
```

---
---

### Examples: 

 1. [ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array)
 2. [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)
 3. [ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple)
 4. [ISR_Changing_PWM](examples/ISR_Changing_PWM)
 5. [ISR_Modify_PWM](examples/ISR_Modify_PWM)
 6. [**multiFileProject**](examples/multiFileProject) **New** 

 
---
---

### Example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)

https://github.com/khoih-prog/MBED_RP2040_Slow_PWM/blob/57d1372367eb92e2c69f66a887c5c939113bef8e/examples/ISR_16_PWMs_Array_Complex/ISR_16_PWMs_Array_Complex.ino#L16-L588

---
---

### Debug Terminal Output Samples

### 1. ISR_16_PWMs_Array_Complex on RaspberryPi Pico

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) to demonstrate how to use multiple PWM channels with complex callback functions, the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods and duty-cycles**


```
Starting ISR_16_PWMs_Array_Complex on RaspberryPi Pico
MBED_RP2040_Slow_PWM v1.3.0
[PWM] _timerNo =  0 , Clock (Hz) =  1000000.00 , _fre (Hz) =  50000.00
[PWM] _count =  0 - 20
[PWM] hardware_alarm_set_target, uS =  20
Starting ITimer OK, micros() = 3320077
Channel : 0	    Period : 1000000		OnTime : 50000	Start_Time : 3321325
Channel : 1	    Period : 500000		OnTime : 50000	Start_Time : 3322698
Channel : 2	    Period : 333333		OnTime : 66666	Start_Time : 3323943
Channel : 3	    Period : 250000		OnTime : 75000	Start_Time : 3325097
Channel : 4	    Period : 200000		OnTime : 80000	Start_Time : 3326457
Channel : 5	    Period : 166666		OnTime : 74999	Start_Time : 3327632
Channel : 6	    Period : 142857		OnTime : 71428	Start_Time : 3328874
Channel : 7	    Period : 125000		OnTime : 68750	Start_Time : 3330002
Channel : 8	    Period : 111111		OnTime : 66666	Start_Time : 3331139
Channel : 9	    Period : 100000		OnTime : 65000	Start_Time : 3332396
Channel : 10	    Period : 66666		OnTime : 46666	Start_Time : 3333625
Channel : 11	    Period : 50000		OnTime : 37500	Start_Time : 3334912
Channel : 12	    Period : 40000		OnTime : 32000	Start_Time : 3336165
Channel : 13	    Period : 33333		OnTime : 28333	Start_Time : 3337497
Channel : 14	    Period : 25000		OnTime : 22500	Start_Time : 3338945
Channel : 15	    Period : 20000		OnTime : 19000	Start_Time : 3340266
SimpleTimer (ms): 2000, us : 13341250, Dus : 10020493
PWM Channel : 0, programmed Period (us): 1000000.00, actual : 1000000, programmed DutyCycle : 5.00, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000.00, actual : 499999, programmed DutyCycle : 10.00, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333.34, actual : 333339, programmed DutyCycle : 20.00, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000.00, actual : 250000, programmed DutyCycle : 30.00, actual : 30.00
PWM Channel : 4, programmed Period (us): 200000.00, actual : 200000, programmed DutyCycle : 40.00, actual : 40.00
PWM Channel : 5, programmed Period (us): 166666.67, actual : 166680, programmed DutyCycle : 45.00, actual : 44.99
PWM Channel : 6, programmed Period (us): 142857.14, actual : 142859, programmed DutyCycle : 50.00, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000.00, actual : 124999, programmed DutyCycle : 55.00, actual : 54.99
PWM Channel : 8, programmed Period (us): 111111.11, actual : 111120, programmed DutyCycle : 60.00, actual : 59.99
PWM Channel : 9, programmed Period (us): 100000.00, actual : 100000, programmed DutyCycle : 65.00, actual : 65.00
PWM Channel : 10, programmed Period (us): 66666.66, actual : 66680, programmed DutyCycle : 70.00, actual : 69.98
PWM Channel : 11, programmed Period (us): 50000.00, actual : 49999, programmed DutyCycle : 75.00, actual : 75.00
PWM Channel : 12, programmed Period (us): 40000.00, actual : 39999, programmed DutyCycle : 80.00, actual : 79.97
PWM Channel : 13, programmed Period (us): 33333.33, actual : 33340, programmed DutyCycle : 85.00, actual : 84.98
PWM Channel : 14, programmed Period (us): 25000.00, actual : 25012, programmed DutyCycle : 90.00, actual : 89.93
PWM Channel : 15, programmed Period (us): 20000.00, actual : 20034, programmed DutyCycle : 95.00, actual : 94.76
SimpleTimer (ms): 2000, us : 23380221, Dus : 10038971
PWM Channel : 0, programmed Period (us): 1000000.00, actual : 1000002, programmed DutyCycle : 5.00, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000.00, actual : 500001, programmed DutyCycle : 10.00, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333.34, actual : 333340, programmed DutyCycle : 20.00, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000.00, actual : 250001, programmed DutyCycle : 30.00, actual : 29.99
PWM Channel : 4, programmed Period (us): 200000.00, actual : 200001, programmed DutyCycle : 40.00, actual : 39.99
PWM Channel : 5, programmed Period (us): 166666.67, actual : 166680, programmed DutyCycle : 45.00, actual : 44.98
PWM Channel : 6, programmed Period (us): 142857.14, actual : 142860, programmed DutyCycle : 50.00, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000.00, actual : 125001, programmed DutyCycle : 55.00, actual : 55.00
PWM Channel : 8, programmed Period (us): 111111.11, actual : 111120, programmed DutyCycle : 60.00, actual : 59.99
PWM Channel : 9, programmed Period (us): 100000.00, actual : 99999, programmed DutyCycle : 65.00, actual : 64.99
PWM Channel : 10, programmed Period (us): 66666.66, actual : 66670, programmed DutyCycle : 70.00, actual : 69.99
PWM Channel : 11, programmed Period (us): 50000.00, actual : 50029, programmed DutyCycle : 75.00, actual : 74.96
PWM Channel : 12, programmed Period (us): 40000.00, actual : 40001, programmed DutyCycle : 80.00, actual : 80.02
PWM Channel : 13, programmed Period (us): 33333.33, actual : 33341, programmed DutyCycle : 85.00, actual : 84.98
PWM Channel : 14, programmed Period (us): 25000.00, actual : 25009, programmed DutyCycle : 90.00, actual : 89.90
PWM Channel : 15, programmed Period (us): 20000.00, actual : 19994, programmed DutyCycle : 95.00, actual : 94.93
SimpleTimer (ms): 2000, us : 33419214, Dus : 10038993
PWM Channel : 0, programmed Period (us): 1000000.00, actual : 1000001, programmed DutyCycle : 5.00, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000.00, actual : 500000, programmed DutyCycle : 10.00, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333.34, actual : 333341, programmed DutyCycle : 20.00, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000.00, actual : 249999, programmed DutyCycle : 30.00, actual : 30.00
PWM Channel : 4, programmed Period (us): 200000.00, actual : 200000, programmed DutyCycle : 40.00, actual : 40.00
PWM Channel : 5, programmed Period (us): 166666.67, actual : 166679, programmed DutyCycle : 45.00, actual : 44.99
PWM Channel : 6, programmed Period (us): 142857.14, actual : 142860, programmed DutyCycle : 50.00, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000.00, actual : 125000, programmed DutyCycle : 55.00, actual : 54.99
PWM Channel : 8, programmed Period (us): 111111.11, actual : 111120, programmed DutyCycle : 60.00, actual : 59.99
PWM Channel : 9, programmed Period (us): 100000.00, actual : 100014, programmed DutyCycle : 65.00, actual : 64.99
PWM Channel : 10, programmed Period (us): 66666.66, actual : 66681, programmed DutyCycle : 70.00, actual : 69.97
PWM Channel : 11, programmed Period (us): 50000.00, actual : 50028, programmed DutyCycle : 75.00, actual : 74.94
PWM Channel : 12, programmed Period (us): 40000.00, actual : 40000, programmed DutyCycle : 80.00, actual : 79.97
PWM Channel : 13, programmed Period (us): 33333.33, actual : 33343, programmed DutyCycle : 85.00, actual : 84.94
PWM Channel : 14, programmed Period (us): 25000.00, actual : 25021, programmed DutyCycle : 90.00, actual : 89.90
PWM Channel : 15, programmed Period (us): 20000.00, actual : 20003, programmed DutyCycle : 95.00, actual : 94.94
```

---

### 2. ISR_16_PWMs_Array on RaspberryPi Pico

The following is the sample terminal output when running example [**ISR_16_PWMs_Array**](examples/ISR_16_PWMs_Array) on **RP2040-based RaspberryPi Pico** to demonstrate how to use multiple PWM channels with simple callback functions.

```
Starting ISR_16_PWMs_Array on RaspberryPi Pico
MBED_RP2040_Slow_PWM v1.3.0
[PWM] _timerNo =  0 , Clock (Hz) =  1000000.00 , _fre (Hz) =  50000.00
[PWM] _count =  0 - 20
[PWM] hardware_alarm_set_target, uS =  20
Starting ITimer OK, micros() = 3736634
Channel : 0	    Period : 1000000		OnTime : 50000	Start_Time : 3737939
Channel : 1	    Period : 500000		OnTime : 50000	Start_Time : 3739363
Channel : 2	    Period : 333333		OnTime : 66666	Start_Time : 3740718
Channel : 3	    Period : 250000		OnTime : 75000	Start_Time : 3741877
Channel : 4	    Period : 200000		OnTime : 80000	Start_Time : 3743177
Channel : 5	    Period : 166666		OnTime : 74999	Start_Time : 3744513
Channel : 6	    Period : 142857		OnTime : 71428	Start_Time : 3745811
Channel : 7	    Period : 125000		OnTime : 68750	Start_Time : 3747053
Channel : 8	    Period : 111111		OnTime : 66666	Start_Time : 3748263
Channel : 9	    Period : 100000		OnTime : 65000	Start_Time : 3749498
Channel : 10	    Period : 66666		OnTime : 46666	Start_Time : 3750692
Channel : 11	    Period : 50000		OnTime : 37500	Start_Time : 3752055
Channel : 12	    Period : 40000		OnTime : 32000	Start_Time : 3753313
Channel : 13	    Period : 33333		OnTime : 28333	Start_Time : 3754675
Channel : 14	    Period : 25000		OnTime : 22500	Start_Time : 3756047
Channel : 15	    Period : 20000		OnTime : 19000	Start_Time : 3757332
```

---

### 3. ISR_16_PWMs_Array_Simple on RaspberryPi Pico

The following is the sample terminal output when running example [**ISR_16_PWMs_Array_Simple**](examples/ISR_16_PWMs_Array_Simple) on **RP2040-based RaspberryPi Pico** to demonstrate how to use multiple PWM channels.

```
Starting ISR_16_PWMs_Array_Simple on RaspberryPi Pico
MBED_RP2040_Slow_PWM v1.3.0
[PWM] _timerNo =  0 , Clock (Hz) =  1000000.00 , _fre (Hz) =  50000.00
[PWM] _count =  0 - 20
[PWM] hardware_alarm_set_target, uS =  20
Starting ITimer OK, micros() = 3635936
Channel : 0	    Period : 1000000		OnTime : 50000	Start_Time : 3637221
Channel : 1	    Period : 500000		OnTime : 50000	Start_Time : 3638549
Channel : 2	    Period : 333333		OnTime : 66666	Start_Time : 3639819
Channel : 3	    Period : 250000		OnTime : 75000	Start_Time : 3641096
Channel : 4	    Period : 200000		OnTime : 80000	Start_Time : 3642434
Channel : 5	    Period : 166666		OnTime : 74999	Start_Time : 3643793
Channel : 6	    Period : 142857		OnTime : 71428	Start_Time : 3645252
Channel : 7	    Period : 125000		OnTime : 68750	Start_Time : 3646599
Channel : 8	    Period : 111111		OnTime : 66666	Start_Time : 3647898
Channel : 9	    Period : 100000		OnTime : 65000	Start_Time : 3649115
Channel : 10	    Period : 66666		OnTime : 46666	Start_Time : 3650427
Channel : 11	    Period : 50000		OnTime : 37500	Start_Time : 3651688
Channel : 12	    Period : 40000		OnTime : 32000	Start_Time : 3652992
Channel : 13	    Period : 33333		OnTime : 28333	Start_Time : 3654391
Channel : 14	    Period : 25000		OnTime : 22500	Start_Time : 3655707
Channel : 15	    Period : 20000		OnTime : 19000	Start_Time : 3657122
```

---

### 4. ISR_Modify_PWM on RaspberryPi Pico

The following is the sample terminal output when running example [ISR_Modify_PWM](examples/ISR_Modify_PWM) on **RP2040-based RaspberryPi Pico** to demonstrate how to modify PWM settings on-the-fly without deleting the PWM channel

```
Starting ISR_Modify_PWM on RaspberryPi Pico
MBED_RP2040_Slow_PWM v1.3.0
[PWM] _timerNo =  0 , Clock (Hz) =  1000000.00 , _fre (Hz) =  50000.00
[PWM] _count =  0 - 20
[PWM] hardware_alarm_set_target, uS =  20
Starting ITimer OK, micros() = 12754395
Using PWM Freq = 200.00, PWM DutyCycle = 1.00
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 12756541
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 22754527
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 32750968
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 42758860
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 52755360
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 62758443
```

---

### 5. ISR_Changing_PWM on RaspberryPi Pico

The following is the sample terminal output when running example [ISR_Changing_PWM](examples/ISR_Changing_PWM) on **RP2040-based RaspberryPi Pico** to demonstrate how to modify PWM settings on-the-fly by deleting the PWM channel and reinit the PWM channel

```
Starting ISR_Changing_PWM on RaspberryPi Pico
MBED_RP2040_Slow_PWM v1.3.0
[PWM] _timerNo =  0 , Clock (Hz) =  1000000.00 , _fre (Hz) =  50000.00
[PWM] _count =  0 - 20
[PWM] hardware_alarm_set_target, uS =  20
Starting ITimer OK, micros() = 3336281
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 3338285
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 12940431
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 22543196
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 32146094
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 41747971
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_PWM_LOGLEVEL_` from 0 to 4

```cpp
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [MBED_RP2040_Slow_PWM issues](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM/issues)

---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards

---

## DONE

 1. Basic hardware multi-channel PWM for **RP2040-based RaspberryPi Pico, Nano_RP2040_Connect, etc.**
 2. Add Table of Contents
 3. Add functions to modify PWM settings on-the-fly
 4. Improve accuracy by using `float`, instead of `uint32_t` for `dutycycle`
 5. Optimize library code by using `reference-passing` instead of `value-passing`
 6. DutyCycle to be optionally updated at the end current PWM period instead of immediately.
 7. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
 8. Display informational warning only when `_PWM_LOGLEVEL_` > 3
 9. Fix `poor-timer-accuracy` bug

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. [Neil Baylis](https://github.com/pixpop) to report relating issue [Poor accuracy on timer interrupt frequency or interval. #4](https://github.com/khoih-prog/MBED_RPI_PICO_TimerInterrupt/issues/4), leading to version v1.3.0 to fix `poor-timer-accuracy` bug

<table>
  <tr>
    <td align="center"><a href="https://github.com/pixpop"><img src="https://github.com/pixpop.png" width="100px;" alt="pixpop"/><br /><sub><b>Neil Baylis</b></sub></a><br /></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/MBED_RP2040_Slow_PWM/blob/main/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


