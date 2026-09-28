# Embedded Greenhouse Temperature Controller

An embedded system for monitoring greenhouse temperature and automatically controlling a cooling fan using the LPC2138 ARM7 microcontroller.

## Project Overview

This project monitors temperature using an LM35 temperature sensor and ADC0804. The LPC2138 processes the digital temperature data and displays the temperature on a 16×2 LCD.

The cooling fan is controlled using PWM according to the measured temperature. UART0 is also used for serial communication at 9600 baud.

The project was developed and simulated using Keil µVision and Proteus.

## Features

- Temperature monitoring using LM35
- ADC0804 interfacing with LPC2138
- 16×2 LCD interfacing in 4-bit mode
- Automatic fan control
- PWM-based fan speed control
- UART0 communication at 9600 baud
- Proteus-based circuit simulation
- Embedded C firmware

## Hardware Used

| Component | Purpose |
|---|---|
| LPC2138 | Main microcontroller |
| LM35 | Temperature sensing |
| ADC0804 | Analog-to-digital conversion |
| 16×2 LCD | Temperature display |
| DC Fan | Cooling |
| PWM2 | Fan speed control |
| UART0 | Serial communication |

## Pin Configuration

### LCD

| LCD Pin | LPC2138 Pin |
|---|---|
| RS | P0.16 |
| EN | P0.17 |
| D4 | P0.18 |
| D5 | P0.19 |
| D6 | P0.20 |
| D7 | P0.21 |

### ADC0804

| ADC0804 Signal | LPC2138 Pin |
|---|---|
| WR | P0.6 |
| RD | P0.8 |
| INTR | P0.9 |
| D0-D7 | P1.16-P1.23 |

### Fan

| Signal | LPC2138 Pin |
|---|---|
| PWM2 | P0.7 |

### UART

| UART Signal | LPC2138 Pin |
|---|---|
| TXD0 | P0.0 |
| RXD0 | P0.1 |

## Software and Tools

- Keil µVision
- Proteus
- Embedded C
- ARM7/LPC2138

## Working

1. The LM35 produces an analog voltage proportional to temperature.
2. ADC0804 converts the analog signal into an 8-bit digital value.
3. LPC2138 reads the ADC0804 output through P1.16-P1.23.
4. The temperature is calculated using the ADC value.
5. The measured temperature is displayed on the 16×2 LCD.
6. The controller determines the required fan speed according to the temperature.
7. PWM2 on P0.7 controls the fan speed.
8. Temperature and fan status are transmitted through UART0 at 9600 baud.

## Temperature-Based Fan Control

| Temperature | Fan Speed |
|---|---|
| Below 25°C | OFF |
| 25°C to below 30°C | 30% |
| 30°C to below 35°C | 50% |
| 35°C to below 40°C | 75% |
| 40°C and above | 100% |

## Temperature Calculation

The program assumes:

- ADC reference voltage = 5 V
- ADC resolution = 8-bit
- LM35 sensitivity = 10 mV/°C

The temperature is calculated in the program using:

    Temperature = ADC_Value × 500 / 255

## Project Structure

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

## How to Use

### Keil

1. Open `AM_PNL_5.uvproj` in Keil µVision.
2. Build the project.
3. Generate the HEX firmware file.
4. The source code is available in the `KEIL` folder.

### Proteus

1. Open the Proteus project from the `PROTEUS` folder.
2. Load the HEX firmware into the LPC2138 if required.
3. Run the simulation.

## Result

The system demonstrates temperature monitoring and automatic fan control using an LPC2138-based embedded system. The temperature is displayed on the LCD, while the fan speed is adjusted using PWM according to the measured temperature.

UART0 provides serial output containing the measured temperature and fan status.

## Learning Outcomes

- ARM7 LPC2138 programming
- ADC0804 interfacing
- LM35 temperature sensing
- LCD interfacing
- PWM-based fan control
- UART communication
- Embedded C programming
- Proteus simulation
- Keil µVision project development

## Author

Jay Rana
