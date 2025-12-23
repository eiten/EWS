# Changelog

> 🌍 **Sprachen:** [English](CHANGELOG.md) | [Deutsch](CHANGELOG.de.md)

Alle wichtigen Änderungen an diesem Projekt werden in dieser Datei dokumentiert.

Das Format basiert auf [Keep a Changelog](https://keepachangelog.com/de/1.0.0/),
und dieses Projekt folgt [Semantic Versioning](https://semver.org/lang/de/).

## [Unreleased]

### Behoben
- **USB-Hub-Controller (U401/CH334):** Pin 4 (XO) auf GND gelegt, um den Betrieb ohne Kristall zu ermöglichen
- **DC/DC-Wandler (U202/XR76208):** Enable-Beschaltung von Spannungsteiler auf Zenerdiode geändert, um 12V-Betrieb zu ermöglichen
- **USB-Ausgänge:** Sicherungen durch TPS2557 Power-Distribution-Switches ersetzt, um Backfeeding zu verhindern

### Geändert
- **Produktion:** Testpunkte TP202/TP203 von Stückliste ausgeschlossen
- **JLCPCB Optimierung:** Rotations-Offsets für automatische Bestückung angepasst
  - U202/U301: FT Rotation Offset 270°
  - U401: FT Rotation Offset 180°

### Technische Details
- **USB-Hub Stabilität:** CH334 arbeitet nun im internen Oszillator-Modus ohne externen Kristall
- **Erweiterte Eingangsspannung:** XR76208 unterstützt nun 12-24V Eingangsspannung durch Zenerdioden-basierte Enable-Schaltung
- **USB-Backfeed-Schutz:** TPS2557 verhindert Rückspeisung von externen USB-Geräten und bietet Überstrom-/Kurzschlussschutz
- **Fertigungseffizienz:** Optimierte Bauteil-Orientierung reduziert Produktionszeit

## [Rev. 0.3] - 2025-12-09

### Hinzugefügt
- **CAN-Transceiver:** 10µF Kondensator C304 für verbesserte Stabilität der CAN-Kommunikation
- **PCB Layout:** QR Code Platzhalter für Seriennummer auf der Platine

### Technische Details
- **Verbesserte Signal-Integrität:** Zusätzliche Filterung am CAN-Transceiver für stabilere Datenübertragung
- **Rückverfolgbarkeit:** QR Code ermöglicht eindeutige Identifikation und Dokumentation

## [Rev. 0.2] - 2025-12-08

### Verbesserungen
- **Schaltplan:** Aufgeräumt und strukturiert (Dank an "shx")
- **CAN-Transceiver:** Silent Pin auf GND gelegt für bessere Stabilität (Dank an "shx")

### Hinzugefügt
- **Status-LEDs:** 
  - Gelbe LEDs für 24V, 5V und 3.3V Spannungsanzeige
  - Grüne LED am Mikrocontroller (Pin PB4)
- **Solder Jumper:** Ersatz der Standard-Jumper durch Solder Jumper auf der Unterseite
- **PFET-Steuerung:** Jumper für Ausgangs-PFETs hinzugefügt - ermöglicht Dauer-Ein ohne Firmware
- **Testpunkte:** Robuste Testpunkte für 5V und GND für Lasttests eingefügt
- **Heatsink:** Optionaler Heatsink für DC/DC-Wandler auf der Unterseite
- **MCU-Kondensator:** 10µF Kondensator für Mikrocontroller an der 3.3V-Schiene

### Geändert
- **Produktion:** Alle LCSC-Bestellnummern eingefügt, Rotationen definiert, Produktionsdaten erstellt
- **PCB Layout:** 
  - Entsprechend der Schaltplanänderungen angepasst
  - Taster besser positioniert
- **BOM optimiert:**
  - 30kΩ durch Basic-Part 33kΩ ersetzt
  - 15.5kΩ am DC/DC-Feedback durch 15kΩ ersetzt → neue Ausgangsspannung: 5.1V
- **CAN-ESD-Schutz:** NUP2105L durch PESD1CAN ersetzt (bessere Eignung)
- **CAN-Abschlusswiderstand:** Bauform von 0402 auf 0603 geändert für ausreichende Leistung
- **USB-Hub:** Speisung von 5V auf 3.3V umgestellt (gemäß Datenblatt-Empfehlung)

### Technische Details
- **DC/DC-Ausgang:** Neue Spannung 5.1V statt 5.25V durch Feedback-Widerstand-Änderung
- **Verbesserte Wärmeableitung:** Optionaler Heatsink auf Unterseite
- **Stabilere CAN-Kommunikation:** Durch ESD-Schutz-Upgrade und Widerstand-Optimierung

---

> **Hinweis:** Dies ist die Originaldokumentation in deutscher Sprache. Eine englische Übersetzung finden Sie in [CHANGELOG.md](CHANGELOG.md).