# Changelog

> 🌍 **Languages:** [English](CHANGELOG.md) | [Deutsch](CHANGELOG.de.md)

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

### Added
- **Documentation:** Added datasheets for all components with an LCSC part number in `doc/datasheets`.

### Fixed
- **USB Hub Controller (U401/CH334):** Pin 4 (XO) connected to GND to enable operation without crystal
- **DC/DC Converter (U202/XR76208):** Enable circuit changed from voltage divider to Zener diode to enable 12V operation
- **USB Outputs:** Fuses replaced with TPS2557 power distribution switches to prevent backfeeding

### Changed
- **Production:** Test points TP202/TP203 excluded from BOM
- **JLCPCB Optimization:** Rotation offsets adjusted for automated assembly
  - U202/U301: FT Rotation Offset 270°
  - U401: FT Rotation Offset 180°

### Technical Details
- **USB Hub Stability:** CH334 now operates in internal oscillator mode without external crystal
- **Extended Input Voltage:** XR76208 now supports 12-24V input voltage through Zener diode-based enable circuit
- **USB Backfeed Protection:** TPS2557 prevents backfeeding from external USB devices and provides overcurrent/short-circuit protection
- **Manufacturing Efficiency:** Optimized component orientation reduces production time

## [Rev. 0.3] - 2025-12-09

### Added
- **CAN Transceiver:** 10µF capacitor C304 for improved CAN communication stability
- **PCB Layout:** QR code placeholder for serial number on the PCB

### Technical Details
- **Enhanced Signal Integrity:** Additional filtering at CAN transceiver for more stable data transmission
- **Traceability:** QR code enables unique identification and documentation

## [Rev. 0.2] - 2025-12-08

### Improvements
- **Schematic:** Cleaned up and structured (Thanks to "shx")
- **CAN Transceiver:** Silent pin connected to GND for better stability (Thanks to "shx")

### Added
- **Status LEDs:** 
  - Yellow LEDs for 24V, 5V and 3.3V voltage indication
  - Green LED on microcontroller (Pin PB4)
- **Solder Jumpers:** Replaced standard jumpers with solder jumpers on bottom side
- **PFET Control:** Added jumpers for output PFETs - enables permanent-on without firmware
- **Test Points:** Added robust test points for 5V and GND for load testing
- **Heatsink:** Optional heatsink for DC/DC converter on bottom side
- **MCU Capacitor:** 10µF capacitor for microcontroller on 3.3V rail

### Changed
- **Production:** Added all LCSC part numbers, defined rotations, created production data
- **PCB Layout:** 
  - Adjusted according to schematic changes
  - Improved button positioning
- **BOM Optimized:**
  - Replaced 30kΩ with basic part 33kΩ
  - Changed 15.5kΩ DC/DC feedback to 15kΩ → new output voltage: 5.1V
- **CAN ESD Protection:** Replaced NUP2105L with PESD1CAN (better suited)
- **CAN Termination Resistor:** Changed footprint from 0402 to 0603 for adequate power rating
- **USB Hub:** Changed supply from 5V to 3.3V (per datasheet recommendation)

### Technical Details
- **DC/DC Output:** New voltage 5.1V instead of 5.25V due to feedback resistor change
- **Improved Heat Dissipation:** Optional heatsink on bottom side
- **Stable CAN Communication:** Through ESD protection upgrade and resistor optimization

---

> **Note:** The original documentation is in German ([CHANGELOG.de.md](CHANGELOG.de.md)). This English version is a translation for international users.