# Smart Home Project

## Features
- **Password-Protected System**: Access controlled by a UART terminal. The door is locked or unlocked using a **servo motor**, based on the password entered.
- **Temperature-Based Fan Control**: The fan's speed (and 2 DC motors) are adjusted according to the temperature:
  - Above 35°C: Fan runs at 100%
  - 30°C - 35°C: Fan runs at 70%
  - 25°C - 30°C: Fan runs at 30%
  - Below 25°C: Fan is turned off
- **LDR-Based Lighting Control**: Automatically controls 2 lamps based on the ambient light level:
  - Low light: Both lamps are on
  - Medium light: One lamp is on
  - High light: Both lamps are off
- **Alarm System**: If the password is entered incorrectly three times, the system triggers an alarm using a buzzer.

## Hardware Components
- **Atmega32 Microcontroller**
- **LM35 Temperature Sensor**
- **LDR Sensor** for ambient light control
- **Servo Motor** for door lock
- **Motor Driver** to control the motors
- **2 Relays** to manage the system power and lights
- **2 Lamps**
- **2 DC Motors**
- **LCD Display** for status updates
- **Buzzer** for the alarm system

## Software Components
- **FreeRTOS**: Real-Time Operating System handling the tasks
  - **Login Task**: Handles user input for password authentication via the UART terminal.
  - **Temperature Monitoring Task**: Reads temperature from the LM35 sensor.
  - **Fan Control Task**: Adjusts fan speed based on temperature readings.
  - **LDR and Lamps Task**: Controls the lamps based on the light level.
  - **Terminal Task**: Provides real-time control and interaction with the system using UART.

## How it Works
1. The user enters a password via the UART terminal. If the password is correct, the door unlocks. If incorrect, after three attempts, the alarm is triggered.
2. The **LM35 sensor** continuously monitors the room temperature and adjusts the fan's speed accordingly.
3. The **LDR sensor** checks the light level and turns on/off the lamps based on the brightness in the room.

## Installation
To run this project, you will need:
1. **Atmel Studio** for writing and flashing code to the Atmega32 microcontroller.
2. **Proteus** for simulation if needed.
3. **FreeRTOS** installed on the Atmega32.


