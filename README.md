# Simple clock using MBED LPC1768

## Features
* Display time (AM/PM)
* Adjust time
* Set alarm. (Indicator if alarm is ON/OFF)
* Initial setup at reset button press.
## Components required
* Liquid Crystal Display (LCD)
* Mbed application board 
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
