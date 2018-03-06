ASPLY
=====

Control block for  the 'Kronasteel Gala 600 inox' kitche hood based on atmega8/48/88/168/328 microcontroller

#Functions:
1. Manage light;
2. Manage 3 speed fan;
3. Adjustable timers for light and fan.

#Timer activation:
1. Hold one of channel button (light or fan speeds)  about 2,5 second till LED indicator starts blink (0.5 sec in turn on and 0.5 sec in turn off state);
2. Count 3 blinks of indicator to set timer for 30 minutes (each blink = 10 minutes);
3. Release the button - the channel function is activated, indicator blinks with low intencive (1 sec in turn on and 1 sec in turn off state) till timer is stopped;
4. Timer can be disabled by clicking same button again;

#Power cord pinout:
1. grey
2. brown
3. black - fan 3
4. blue - fan 2
5. red - fan 1
6. white - light

#Usefull link:
1. https://github.com/MCUdude/MiniCore - supporting of atmega 8/48/88/168 for ArduinoIDE;
2. https://github.com/dxinteractive/AnalogMultiButton - the library of buttons states and debouner for Arduino IDE.

#Flash atmega48:
1. Burn 'Arduino as ISP' sketch;
2. Pin Arduino board to atmega48 board on a breadboard;
3. Set Tools -> Programmer -> Arduino as ISP;
4. Set Tools -> Board -> Atmega48;
5. Set Tools -> Clock -> 8MHz internal;
6. Flash controller by Sketch -> Upload Using Programmer.

#Trouble shooting
If clear atmega48/88 is used and it works too slowly, you need to set correct fuse:

```
avrdude -c avrisp -p m48p -P /dev/cu.usbmodem1411 -b 19200 -U lfuse:w:0xE2:m -v
```

#Thanks for help
Виктор ! Михалин <vik-lab@mail.ru>
