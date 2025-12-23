# 🔌 Universal Voron Power & CAN HAT (EWS)

> 🌍 **Languages:** [English](README.md) | [Deutsch](README.de.md)

A universal interface board (HAT) for Raspberry Pi 4/5 and Radxa Rock 5B designed for use in Voron 3D printers. Provides powerful 5V supply (8A), CAN-Bus interface (USB-Bridge or Native), and USB hub.

![Universal Voron Power & CAN HAT](img/EWS.png)

## 📚 Contents

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [🔌 Universal Voron Power & CAN HAT (EWS)](#-universal-voron-power--can-hat-ews)
  - [📚 Contents](#-contents)
  - [🎯 About EWS](#-about-ews)
  - [🚨 Problems Addressed](#-problems-addressed)
  - [✨ Features](#-features)
    - [🔋 Power Supply (24V Input)](#-power-supply-24v-input)
    - [⚡ DC/DC Converter (5V Output)](#-dcdc-converter-5v-output)
    - [🔌 Logic Power Supply (Power Path)](#-logic-power-supply-power-path)
    - [💻 MCU & CAN Interface](#-mcu--can-interface)
    - [📡 USB Hub](#-usb-hub)
  - [📋 Klipper Configuration](#-klipper-configuration)
    - [Basic MCU Setup](#basic-mcu-setup)
  - [🔌 Pinout & Connector Assignment](#-pinout--connector-assignment)
    - [Power Input](#power-input)
    - [CAN-Bus Ports](#can-bus-ports)
    - [USB Ports](#usb-ports)
  - [🛠️ PCB Specifications](#️-pcb-specifications)
  - [⚠️ Manufacturing Recommendations](#️-manufacturing-recommendations)
    - [PCB Manufacturing](#pcb-manufacturing)
    - [Assembly Notes](#assembly-notes)
    - [Component Sourcing](#component-sourcing)
  - [🛠️ Technical Specifications](#️-technical-specifications)
  - [📚 Additional Resources](#-additional-resources)
  - [📝 Changelog](#-changelog)
  - [License](#license)

<!-- /code_chunk_output -->

## 🎯 About EWS

**EWS** stands for "Eierlegende Wollmilchsau" (German for "egg-laying woolly milk pig") - a humorous term for a universal solution that does everything. This HAT lives up to its nickname by combining multiple essential functions for Voron 3D printer control systems in a single, compact board.

The board addresses common pain points in Voron builds by providing robust power distribution, reliable CAN communication, and convenient USB connectivity while maintaining compatibility with both Raspberry Pi and Radxa Rock 5B single-board computers.

## 🚨 Problems Addressed

This board was designed to solve multiple critical issues commonly encountered in high-performance 3D printer setups:

### ⚡ **Power & Supply Issues**
- **Unstable SBC power supply** - Many setups suffer from voltage drops and power instability
- **Separate power injection required** - USB-C touchscreens often need additional power supplies
- **Power distribution complexity** - Multiple power sources create wiring challenges

### 🔗 **CAN Communication Problems**
- **USB-CAN adapters** - Difficult to mount, unreliable connections, additional failure points
- **Controller board limitations** - Using Spider/Octopus as CAN bridge often has built-in termination resistors
- **CAN topology conflicts** - Can't run both CAN toolheads and ADXL boards simultaneously on many controller boards
- **Native CAN unavailable** - Most SBC setups lack proper native CAN interface utilization

### 🔥 **Safety Concerns**
- **Fire hazard from thermal runaway** - No way to disconnect toolhead power during emergencies
- **Uncontrolled power delivery** - Toolheads remain powered even during faults
- **Limited emergency shutdown** - Difficult to implement comprehensive safety cutoffs

### 🔌 **Wiring & Installation**
- **Complex cable management** - CAN and 24V power start from different locations
- **Multiple connection points** - Increases failure modes and installation complexity
- **Inadequate connector standards** - Mix of different connector types creates confusion

### 🖥️ **USB & Peripheral Issues**
- **Limited USB ports** - SBCs often lack sufficient high-power USB connectivity
- **USB hub limitations** - External hubs add complexity and potential points of failure
- **Power delivery standards** - Inconsistent USB-C power delivery for modern touchscreens

## ✨ Features

### 🔋 Power Supply (24V Input)

- **Input Voltage:** 24V DC nominal
- **Connectors:** XT30PW-F (horizontal) or 5.08mm screw terminal
- **Toolhead Switching:** 2x P-Channel MOSFETs (CJAC70P06, -60V, -70A, RDSon ~8mΩ) controlled by PB2 and PA5 for toolhead power switching (safety feature if PWM-FET on toolhead fails)
- **Overvoltage Protection:** TVS diode (SMAJ26A, 26V Standoff, Unidirectional) to GND
- **Input Protection:** Distributed fusing (5A for DC/DC, 5A each for CAN ports)

### ⚡ DC/DC Converter (5V Output)

- **Controller:** MaxLinear XR76208 (Synchronous Step-Down, 8A, COT)
- **Input Voltage:** 12-24V DC (enable circuit with Zener diode)
- **Input Fuse:** 5A SMD 1812 Slow Blow
- **Reverse Polarity Protection:** SS56 Schottky diode
- **Output Voltage:** 5.1V (set via feedback divider: R_Top=15kΩ, R_Bottom=2.0kΩ)
- **Switching Frequency:** ~600kHz (set via Ron=30kOhm)
- **Inductance:** 3.3µH Shielded (Sunlord MDA1050-3R3M, Isat ~17A)
- **Input Capacitors:** 4x 10µF 1206 ceramic + 1x 100µF electrolytic/polymer (bulk)
- **Output Capacitors:** 4x 22µF 1206 ceramic
- **Stability:** Feed-forward capacitor (Cff) 1nF parallel to upper feedback resistor
- **Soft-Start:** 100nF at SS pin (~6ms startup time)
- **Current Limit:** 5.1kΩ resistor (~10.2A limit)
- **Enable:** Zener diode-based circuit for 12-24V input voltage range

### 🔌 Logic Power Supply (Power Path)

- **Concept:** Separation of +5V_PWR (high current for screen/webcam) and +5V_LOGIC (for MCU/Hub)
- **Diode OR-ing:** 2x Schottky diodes (1N5819WS, 1A)
  - Source 1: USB_VBUS (from Pi/PC) → Diode → +5V_LOGIC
  - Source 2: +5V_BUCK (from 8A regulator) → Diode → +5V_LOGIC
- **Purpose:** Board is USB-flashable without 24V supply, no backflow to PC
- **3.3V Generation:** LDO (XC6206) powered from +5V_LOGIC

### 🧠 MCU & CAN Interface

- **Microcontroller:** STM32G0B1KBU6 (UFQFPN-32)
- **Clocking:** Crystal-less (Internal HSI48 with Clock Recovery System CRS via USB)
- **Pi Connection:**
  - USB (PA11/PA12) to USB Hub
  - UART (PA9/PA10) to Pi Header (Pin 8/10) via jumperable connection
  - NRST to Pi GPIO 22 (via 1kΩ) + button to GND + 100nF cap
  - BOOT0 (Pin 24/PA14 shared with SWCLK) to Pi GPIO 27 (via 1kΩ) + button to 3.3V + 10kΩ pull-down
- **CAN Transceiver:** MCP2542WFD (or compatible)
- **Muxing:** Jumper selectable between STM32 (USB-CAN Bridge) and Rock/Pi Native GPIOs (Pin 3/5). For ROCK 5B/5B+, jumpers can select whether CAN comes from onboard STM32G0B1 or goes natively to Rockchip (via GPIO header Pin 3 (CAN-RX) and Pin 5 (CAN-TX))
- **Termination:** 120 Ω resistor, switchable per jumper (directly at connectors)
- **CAN Connectors:** 2x Molex Micro-Fit 3.0 (2x2)
- **Micro-Fit Pinout:** Pin 1=24V, Pin 2=GND (upper row), Pin 3=CAN_H, Pin 4=CAN_L (lower row)
- **CAN Power Protection:** 5A SMD 1812 Slow Blow per port

### 📡 USB Hub

- **Controller:** WCH CH334F (QFN-24)
- **Speed:** USB 2.0 High Speed (480 Mbit/s) with MTT
- **Clocking:** Internal (crystal-less), XO pin connected to GND for internal oscillator mode
- **Supply:** V5 to +5V_LOGIC
- **Upstream:** To Raspberry Pi Header (USB loopback cable required) or USB-C input
- **Downstream Ports:**
  - STM32 (internal)
  - USB-C connector (for touchscreen)
  - USB-A connector (vertical, for webcam)

**USB Port Protection:**
- **Concept:** 100µF electrolytic tank at +5V_PWR, followed by TPS2557 power distribution switch, followed by 22µF ceramic at connector
- **USB-C Port (Screen):**
  - TPS2557: Power distribution switch with backfeed protection, overcurrent/short-circuit protection
  - Current limiting: Programmable limit via external resistor
  - CC Lines: CC1 and CC2 each with 10kΩ resistor to VBUS (behind switch) for 3A advertisement
  - ESD Protection: SRV05-4 TVS array
- **USB-A Port (Webcam):**
  - TPS2557: Power distribution switch with backfeed protection, overcurrent/short-circuit protection
  - Current limiting: Programmable limit via external resistor
  - ESD Protection: SRV05-4 TVS array

## 📋 Klipper Configuration

### Basic MCU Setup
```ini
[mcu hat]
canbus_uuid: your_uuid_here
# Alternative for USB connection:
# serial: /dev/serial/by-id/usb-katapult_stm32g0b1xx_your_id-if00

[temperature_sensor hat_mcu]
sensor_type: temperature_mcu
sensor_mcu: hat
```

## 🔌 Pinout & Connector Assignment

### Power Input
- **XT30PW-F (horizontal)** or **5.08mm Screw Terminal**
  - Pin 1: +24V
  - Pin 2: GND

### CAN-Bus Ports
- **2x Molex Micro-Fit 3.0 (2x2, THT)**
  - Pin 1: +24V (upper row, left)
  - Pin 2: GND (upper row, right)
  - Pin 3: CAN_H (lower row, left)
  - Pin 4: CAN_L (lower row, right)

### USB Ports
- **USB-C:** Touchscreen connection (5V/3A capability)
- **USB-A:** Webcam connection (vertical mount)

## 🛠️ PCB Specifications

| Parameter | Specification |
|-----------|---------------|
| **Form Factor** | Standard HAT (65mm x 56mm) |
| **Layer Stack** | 4-Layer Multilayer |
| **Layer 1 (Top)** | Signal/Power |
| **Layer 2** | GND Plane |
| **Layer 3** | 5V Power Plane |
| **Layer 4 (Bottom)** | Signal/GND |
| **Solder Mask** | Black Matte |
| **Surface Finish** | ENIG (Gold) |
| **Silkscreen** | White |
| **Mounting** | Inner mounting holes (Pi/Rock compatible) |
| **EMI Design** | Mounting holes via-stitched to GND planes (Layer 2 & Bottom), isolated from 5V plane (Layer 3) |

## ⚠️ Manufacturing Recommendations

### PCB Manufacturing
- **Copper Weight:** **2oz copper recommended** for improved thermal performance and current handling
- **Via Specifications:** Minimum 12-20 vias at DC/DC PGND for thermal management
- **Impedance Control:** 90 Ω differential for USB traces (D+/D- pairs)

### Assembly Notes
- **24V Path:** Minimum 3mm trace width on top & bottom layers for 12A total current
- **5V Distribution:** Use Layer 3 plane with solid connections (no thermals) to headers and capacitors
- **Thermal Management:** QFN GND pads connected with vias, considering bottom layer routing

### Component Sourcing
- Ensure polyfuse derating for chamber temperatures up to 60°C
- Select inductors with sufficient saturation current (>15A recommended)
- Use low-ESR capacitors for switching regulator

## 🛠️ Technical Specifications

| Parameter | Value | Unit |
|-----------|-------|------|
| **Input Voltage** | 24 ± 2 | V |
| **5V Output Current** | 8 | A |
| **Logic Supply Current** | 2 | A |
| **CAN Baud Rate** | up to 1 | Mbit/s |
| **USB Speed** | 480 | Mbit/s |
| **Operating Temperature** | -10 to +70 | °C |
| **Dimensions** | 65 x 56 | mm |

## 📚 Additional Resources

- [Component Datasheets](doc/datasheets/README.md)
- [Klipper Documentation](https://www.klipper3d.org/Config_Reference.html)
- [CAN-Bus Setup Guide](https://www.klipper3d.org/CANBUS.html)
- [Voron Documentation](https://docs.vorondesign.com/)
- [STM32G0 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0454-stm32g0x0-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [Katapult Firmware Flasher](https://github.com/Arksine/katapult)

## 📝 Changelog

For detailed information about changes and updates, see [CHANGELOG.md](CHANGELOG.md).

## License

This project is licensed under the **Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License** (CC BY-NC-SA 4.0).

[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)

This means you are free to:
- **Share** — copy and redistribute the material in any medium or format
- **Adapt** — remix, transform, and build upon the material

Under the following terms:
- **Attribution** — You must give appropriate credit, provide a link to the license, and indicate if changes were made
- **NonCommercial** — You may not use the material for commercial purposes
- **ShareAlike** — If you remix, transform, or build upon the material, you must distribute your contributions under the same license

For the full license text, visit: https://creativecommons.org/licenses/by-nc-sa/4.0/

---

> **Note:** The original documentation is in German ([README.de.md](README.de.md)). This English version is a translation for international users.