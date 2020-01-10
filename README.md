# ArduinoDueCounter
Code for a counter that outputs the time difference between two input signals. Works on an Arduino Due with TC8 (Channel 2 Counter 2 )

* Capture Pins are 11(TIOA) and 12(TIOB) on the Arduino Due 
* To test simply short the Pins 7,11 and 12 together so you can see the duty cycle of a PWM on Pin 7. 

Copied as much as possible from the arduino forum [0].

There are two versions in the repository. One for finding a fault where the complete Status register is dumped and one that is the bare minimum so you know what is needed. 

The datasheet [1] offers all the sources I needed, most importantly Figure 36-5 Capture Mode:
![Capture Mode](https://raw.githubusercontent.com/petl/ArduinoDueCounter/master/Fig36.5._Capture_Mode.PNG)

[0] https://forum.arduino.cc/index.php?topic=400516.0

[1] https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf

