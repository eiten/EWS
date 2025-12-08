# CAN FD CDC Composite Firmware

A Zephyr-based firmware for the EWS board that provides:
- CAN FD to gs_usb interface (similar to candlelight)
- USB CDC interface for PFET control
- USB composite device functionality

## Features

- **CAN FD Support**: Full CAN FD protocol support via gs_usb interface
- **USB Composite**: Single USB device with multiple interfaces:
  - gs_usb interface for CAN communication
  - CDC ACM interface for PFET control and status
- **PFET Control**: Control both output PFETs via CDC commands
- **Status LEDs**: Control status LEDs for visual feedback
- **Compatible**: Works with standard CAN utilities (can-utils, etc.)

## Hardware Target

- **MCU**: STM32G0B1KBU6 (on EWS board)
- **CAN**: CAN FD via FDCAN1 (PB0/PB1) with SN65HVD230 transceiver
- **USB**: USB 2.0 Full Speed (PA11/PA12)
- **GPIOs**: 
  - PFET1 control: PA8
  - PFET2 control: PB2  
  - Status LED: PB4

## Build Requirements

- Zephyr RTOS (v3.5+)
- West build tool
- ARM GCC toolchain

## Building

```bash
west build -b ews_board
```

## CDC Protocol

The CDC interface uses simple text commands:
- `PFET1_ON\n` - Turn on PFET1
- `PFET1_OFF\n` - Turn off PFET1  
- `PFET2_ON\n` - Turn on PFET2
- `PFET2_OFF\n` - Turn off PFET2
- `STATUS\n` - Get current PFET status