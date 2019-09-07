# Arduino-Laser-Messages
Code for the transmission and reception of information over laser beams written for Arduino

### Setup

First, some things you need:
- 2 * Arduino Uno or similar. (anything with an analogue read pin will probably work)
- LDR (light dependent resistor)
- 10k ohm resistor
- a laser diode

1. Using the wiring diagram, assemble the circuits for the transmitter and receiver.
2. Upload laser_commsV2.ino to both of the Arduino.
3. Point the laser diode of the transmitter at the LDR on the receiver.
4. Connect to the Arduino's over serial and set the receiver and transmitter accordingly

Now send a message over serial to the transmitter and watch it get sent over to the receiver as a laser pulse.


### Demonstration

https://www.youtube.com/watch?v=RrdIopKH7GI
