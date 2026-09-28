# Embedded Greenhouse Temperature Controller

An embedded system for monitoring greenhouse temperature and controlling a cooling fan automatically using the LPC2138 microcontroller.

## Project Overview

This project monitors temperature using an ADC0804 and displays the measured value on a 16×2 LCD. Based on the temperature condition, the system controls a fan using PWM.

The project was developed and tested using Keil µVision and Proteus simulation.

## Features

- Temperature monitoring using ADC0804
- LPC2138 ARM7 microcontroller
- 16×2 LCD temperature display
- Automatic fan control
- PWM-based fan speed control
- UART communication at 9600 baud
- Proteus-based circuit simulation
- Embedded C firmware

## Hardware Used

| Component | Purpose |
|---|---|
| LPC2138 | Main microcontroller |
| ADC0804 | Analog-to-digital conversion |
| Temperature Sensor | Temperature measurement |
| 16×2 LCD | Temperature display |
| DC Fan | Cooling |
| PWM | Fan speed control |
| UART | Serial communication |

## Software and Tools

- Keil µVision
- Proteus
- Embedded C
- ARM7/LPC2138

## Working

1. The temperature sensor produces an analog signal according to the temperature.
2. ADC0804 converts the analog signal into digital data.
3. LPC2138 reads the digital output from the ADC.
4. The measured temperature is displayed on the 16×2 LCD.
5. The controller compares the temperature with the programmed condition.
6. The cooling fan is controlled using PWM.
7. UART communication is used for serial data transmission.

## Project Structure

```text
embedded-greenhouse-temperature-controller/
│
├── KEIL/
│   ├── AM_PNL_5.hex
│   ├── AM_PNL_5.uvproj
│   ├── main.c
│   └── Startup.s
│
└── PROTEUS/
    └── JB_AM_PBL_5.pdsprj
