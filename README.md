# Arduino-Wireless-Communication
This repository contains the Arduino sketches for establishing wireless communication between an Arduino Nano and an Arduino Uno using nRF24L01 modules. The Arduino Nano acts as the master and sends data to the Arduino Uno, while the Arduino Uno serves as the slave and receives data from the Arduino Nano.

## Features:
### Arduino Nano (MasterSwapRoles):
- Reads analog data from a sensor connected to pin AO.
- Sends the analog data to the Arduino Uno using the nRF24L01 module.
- Receives data from the Arduino Uno, adjusts a servo motor based on the received data.
- Periodically sends data to the Arduino Uno.

### Arduino Uno (SlaveSwapRoles):
- Receives data from the Arduino Nano using the nRF24L01 module.
- Displays the received data on an LCD.
- Sends analog data from joystick sensors to the Arduino Nano as a reply.
- Monitors for acknowledgment from the Arduino Nano.

## Hardware Requirements:
1. Arduino Nano
2. Arduino Uno
3. nRF24L01 wireless modules
4. DHT11 sensor (optional for additional functionality)
5. LCD display (optional for additional functionality)
6. Joystick sensors (optional for additional functionality)
7. Servo motor (optional for additional functionality)
