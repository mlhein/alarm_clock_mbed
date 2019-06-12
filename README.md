# Simple clock using MBED LPC1768

## Description
This group project, an assignment for subject Embedded System Programming in University of Malaya. The concept is using Ticker and Interrupt to perform specific functions. Ticker is mainly for updates/shows/blinks the display. Interrupt is mainly for changing the cursor location and select function. Using built-in C++ functions (localtime & set_time) for the time function. In additiona, the code also shows certain values on terminal (using putty/teraterm) for debugging.
## Features
* Display time (AM/PM)
* Adjust time
* Set alarm. (Indicator if alarm is ON/OFF)
* Initial setup at reset button press.
## Components required
* Liquid Crystal Display (LCD)
* Mbed application board (JOYSTICK & BUZZER)
* Mbed LPC178
* Solderless Breadboard
* Wires
## Connections between LCD pins and MBED pins

LCD Pin number|Text LCD pins|MBED pins
--------------|-------------|---------|
1|GND|0V (GRD)
2|VCC|3.3V
3|VO|0V, via 1k resistor
4|RS|P19 
5|RW|0V (GRD)
6|E|P20
7|D0|not connected
8|D1|not connected
9|D2|not connected
10|D3|not connected
11|D4|P25
12|D5|P22
13|D6|P23
14|D7|P24

## Dependencies
- [MBED official library]([[https://os.mbed.com/users/mbed_official/code/mbed/](https://os.mbed.com/users/mbed_official/code/mbed/)])
- [TextLCD by Simon]([https://os.mbed.com/users/simon/code/TextLCD/](https://os.mbed.com/users/simon/code/TextLCD/))

## DEMO
- Starting state.
![Alt](https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/1.jpg)
* Setup time for the first time.
![https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/2.jpg](https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/2.jpg)
* Display time (User able to set alarm or adjust time)
![https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/3.jpg](https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/3.jpg)
* Setup alarm
![https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/4.jpg](https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/4.jpg)
* Indicator alarm is ON (* left next to Alarm)
![https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/5.jpg](https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/5.jpg)
* Adjust time (with Back to cancel)
![https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/6.jpg](https://raw.githubusercontent.com/mlhein/alarm_clock_mbed/master/img_demo/6.jpg)

## Authors
- Mulhein
- Hafizul
- Ebrahim

## Etc.
* Link to original repository: [https://os.mbed.com/users/ebrahimatya/code/grpass_clock/](https://os.mbed.com/users/ebrahimatya/code/grpass_clock/)
