# EWS Board

## Overview

The EWS Board is based on the STM32G0B1KBU6 microcontroller in UFQFPN32 package.

## Hardware Features

- STM32G0B1KBU6 MCU (Arm Cortex-M0+ core, 128 KB Flash, 36 KB RAM)
- Internal HSI oscillator with USB clock recovery
- USB 2.0 Full Speed interface
- CAN FD interface  
- Status LED on PB4
- Two PFET control outputs (PA8, PB2)

## Pin Configuration

| Function | Pin | Notes |
|----------|-----|-------|
| Status LED | PB4 | Active high |
| PFET1 Control | PA8 | Active high |
| PFET2 Control | PB2 | Active high |
| CAN RX | PB0 | FDCAN1_RX |
| CAN TX | PB1 | FDCAN1_TX |
| USB D- | PA11 | USB_DM |
| USB D+ | PA12 | USB_DP |

## Clock Configuration

The board uses the internal HSI oscillator (16 MHz) with PLL to generate:
- System clock: 64 MHz
- USB clock: 48 MHz (from PLL Q output)
- CAN clock: 64 MHz

No external crystal is used; USB clock recovery ensures accurate timing for USB communication.

## Programming and Debugging

The board supports programming via:
- USB DFU (built-in STM32 bootloader)
- SWD interface (if exposed)

## Building Firmware

```bash
west build -b ews
```