# 🔌 Universal Voron Power & CAN HAT (EWS)

> 🌍 **Sprachen:** [English](README.md) | [Deutsch](README.de.md)

Ein universelles Interface-Board (HAT) für Raspberry Pi 4/5 und Radxa Rock 5B zur Verwendung in Voron 3D-Druckern. Bietet eine leistungsstarke 5V-Versorgung (8A), CAN-Bus-Interface (USB-Bridge oder Native) und USB-Hub.

![Universal Voron Power & CAN HAT](img/EWS.png)

## 📚 Inhalt

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [🔌 Universal Voron Power & CAN HAT (EWS)](#-universal-voron-power--can-hat-ews)
  - [📚 Inhalt](#-inhalt)
  - [🎯 Über EWS](#-über-ews)
  - [🚨 Gelöste Probleme](#-gelöste-probleme)
  - [✨ Features](#-features)
    - [🔋 Stromversorgung (24V Eingang)](#-stromversorgung-24v-eingang)
    - [⚡ DC/DC Wandler (5V Ausgang)](#-dcdc-wandler-5v-ausgang)
    - [🔌 Logik-Spannungsversorgung (Power Path)](#-logik-spannungsversorgung-power-path)
    - [💻 MCU & CAN Interface](#-mcu--can-interface)
    - [📡 USB Hub](#-usb-hub)
  - [📋 Klipper Konfiguration](#-klipper-konfiguration)
    - [Basis MCU Setup](#basis-mcu-setup)
  - [🔌 Pinout & Steckerbelegung](#-pinout--steckerbelegung)
    - [Stromeingang](#stromeingang)
    - [CAN-Bus Anschlüsse](#can-bus-anschlüsse)
    - [USB Anschlüsse](#usb-anschlüsse)
  - [🛠️ PCB Spezifikationen](#️-pcb-spezifikationen)
  - [⚠️ Fertigungsempfehlungen](#️-fertigungsempfehlungen)
    - [PCB-Fertigung](#pcb-fertigung)
    - [Bestückungshinweise](#bestückungshinweise)
    - [Bauteilbeschaffung](#bauteilbeschaffung)
  - [🛠️ Technische Spezifikationen](#️-technische-spezifikationen)
  - [📚 Weitere Ressourcen](#-weitere-ressourcen)
  - [Lizenz](#lizenz)

<!-- /code_chunk_output -->

## 🎯 Über EWS

**EWS** steht für "Eierlegende Wollmilchsau" - ein humorvoller Begriff für eine Universallösung, die alles kann. Dieses HAT wird seinem Spitznamen gerecht, indem es mehrere essentielle Funktionen für Voron 3D-Drucker-Steuerungssysteme in einem einzigen, kompakten Board vereint.

Das Board adressiert häufige Problemstellen in Voron-Builds durch robuste Stromverteilung, zuverlässige CAN-Kommunikation und praktische USB-Konnektivität bei gleichzeitiger Kompatibilität mit Raspberry Pi und Radxa Rock 5B Single-Board-Computern.

## 🚨 Gelöste Probleme

Dieses Board wurde entwickelt, um mehrere kritische Probleme zu lösen, die häufig bei Hochleistungs-3D-Drucker-Setups auftreten:

### ⚡ **Strom- & Versorgungsprobleme**
- **Instabile SBC-Stromversorgung** - Viele Setups leiden unter Spannungsabfällen und Strominstabilität
- **Separate Power Injection erforderlich** - USB-C-Touchscreens benötigen oft zusätzliche Netzteile
- **Komplexe Stromverteilung** - Mehrere Stromquellen erschweren die Verkabelung

### 🔗 **CAN-Kommunikationsprobleme**
- **USB-CAN-Adapter** - Schwer zu montieren, unzuverlässige Verbindungen, zusätzliche Fehlerquellen
- **Controller-Board-Limitationen** - Spider/Octopus als CAN-Bridge haben oft eingebaute Abschlusswiderstände
- **CAN-Topologie-Konflikte** - CAN-Toolheads und ADXL-Boards können nicht gleichzeitig an vielen Controller-Boards betrieben werden
- **Native CAN nicht verfügbar** - Die meisten SBC-Setups nutzen keine nativen CAN-Interfaces

### 🔥 **Sicherheitsbedenken**
- **Brandgefahr durch Thermal Runaway** - Keine Möglichkeit, Toolhead-Strom bei Notfällen zu trennen
- **Unkontrollierte Stromversorgung** - Toolheads bleiben auch bei Fehlern unter Strom
- **Begrenzte Notabschaltung** - Schwierig, umfassende Sicherheitsabschaltungen zu implementieren

### 🔌 **Verkabelung & Installation**
- **Komplexes Kabelmanagement** - CAN und 24V-Strom beginnen an verschiedenen Stellen
- **Mehrere Anschlusspunkte** - Erhöht Fehlermöglichkeiten und Installationskomplexität
- **Unzureichende Steckernormen** - Mix verschiedener Steckertypen schafft Verwirrung

### 🖥️ **USB- & Peripherie-Probleme**
- **Begrenzte USB-Anschlüsse** - SBCs haben oft unzureichende Hochleistungs-USB-Konnektivität
- **USB-Hub-Limitationen** - Externe Hubs erhöhen Komplexität und potentielle Fehlerquellen
- **Power-Delivery-Standards** - Inkonsistente USB-C-Stromversorgung für moderne Touchscreens

## ✨ Features

### 🔋 Stromversorgung (24V Eingang)

- **Eingangsspannung:** 24V DC nominal
- **Anschlüsse:** XT30PW-F (liegend) oder 5.08mm Schraubklemme
- **Toolhead-Schaltung:** 2x P-Channel MOSFETs (CJAC70P06, -60V, -70A, RDSon ca. 8mΩ) gesteuert durch PB2 und PA5 zum Ein-/Ausschalten der Toolheads (Sicherheitsfeature, falls PWM-FET am Toolhead durchlegiert)
- **Überspannungsschutz:** TVS-Diode (SMAJ26A, 26V Standoff, Unidirektional) gegen GND
- **Eingangssicherung:** Verteilte Absicherung (5A für DC/DC, je 5A für CAN-Anschlüsse)

### ⚡ DC/DC Wandler (5V Ausgang)

- **Controller:** MaxLinear XR76208 (Synchroner Step-Down, 8A, COT)
- **Eingangssicherung:** 5A SMD 1812 Slow Blow
- **Verpolschutz:** SS56 Schottky-Diode
- **Ausgangsspannung:** 5.25V (eingestellt über Feedback-Teiler: R_Top=15.5kΩ, R_Bottom=2.0kΩ)
- **Schaltfrequenz:** ca. 600kHz (eingestellt über Ron=30kOhm)
- **Induktivität:** 3.3µH Shielded (Sunlord MDA1050-3R3M, Isat ca. 17A)
- **Eingangskondensatoren:** 4x 10µF 1206 Keramik + 1x 100µF Elektrolyt/Polymer (Bulk)
- **Ausgangskondensatoren:** 4x 22µF 1206 Keramik
- **Stabilität:** Feed-Forward Kondensator (Cff) 1nF parallel zum oberen Feedback-Widerstand
- **Soft-Start:** 100nF an Pin SS (ca. 6ms Anlaufzeit)
- **Stromlimit:** 5.1kΩ Widerstand (~10.2A Grenzwert)
- **Enable:** Spannungsteiler von 24V (100kΩ oben, 22kΩ unten) = ca. 4.3V am Pin

### 🔌 Logik-Spannungsversorgung (Power Path)

- **Konzept:** Trennung in +5V_PWR (Hochstrom für Screen/Webcam) und +5V_LOGIC (für MCU/Hub)
- **Dioden-Weiche (ORing):** 2x Schottky-Dioden (1N5819WS, 1A)
  - Quelle 1: USB_VBUS (vom Pi/PC) → Diode → +5V_LOGIC
  - Quelle 2: +5V_BUCK (vom 8A Regler) → Diode → +5V_LOGIC
- **Zweck:** Board ist per USB flashbar ohne 24V-Versorgung, kein Rückstrom in den PC
- **3.3V Erzeugung:** LDO (XC6206) gespeist aus +5V_LOGIC

### 🧠 MCU & CAN Interface

- **Mikrocontroller:** STM32G0B1KBU6 (UFQFPN-32)
- **Taktung:** Crystal-less (Interner HSI48 mit Clock Recovery System CRS über USB)
- **Pi-Verbindung:**
  - USB (PA11/PA12) an USB-Hub
  - UART (PA9/PA10) an Pi-Header (Pin 8/10) über Jumper trennbar
  - NRST an Pi GPIO 22 (via 1kΩ) + Taster gegen GND + 100nF Cap
  - BOOT0 (Pin 24/PA14 shared mit SWCLK) an Pi GPIO 27 (via 1kΩ) + Taster gegen 3.3V + 10kΩ Pull-Down
- **CAN-Transceiver:** MCP2542WFD (oder kompatibel)
- **Muxing:** Jumper wählbar zwischen STM32 (USB-CAN Bridge) und Rock/Pi Native GPIOs (Pin 3/5). Bei ROCK 5B/5B+ kann über Jumper gewählt werden, ob CAN vom Onboard-STM32G0B1 kommt oder nativ an den Rockchip geht (über GPIO-Leiste Pin 3 (CAN-RX) und Pin 5 (CAN-TX))
- **Terminierung:** 120 Ω Widerstand, zuschaltbar per Jumper (direkt an den Buchsen)
- **CAN-Anschlüsse:** 2x Molex Micro-Fit 3.0 (2x2)
- **Micro-Fit Pinout:** Pin 1=24V, Pin 2=GND (Obere Reihe), Pin 3=CAN_H, Pin 4=CAN_L (Untere Reihe)
- **CAN-Stromabsicherung:** Je 5A SMD 1812 Slow Blow pro Port

### 📡 USB Hub

- **Controller:** WCH CH334F (QFN-24)
- **Geschwindigkeit:** USB 2.0 High Speed (480 Mbit/s) mit MTT
- **Taktung:** Intern (Crystal-less), XI/XO Pins offen gelassen
- **Versorgung:** V5 an +5V_LOGIC
- **Upstream:** Zum Raspberry Pi Header (USB Loopback Kabel erforderlich) oder USB-C Input
- **Downstream Ports:**
  - STM32 (Intern)
  - USB-C Buchse (für Touchscreen)
  - USB-A Buchse (Vertikal, für Webcam)

**USB Port Absicherung:**
- **Konzept:** 100µF Elko als Tank an +5V_PWR, gefolgt von Polyfuse, gefolgt von 22µF Keramik an der Buchse
- **USB-C Port (Screen):**
  - Polyfuse: 4.0A Hold Current (Derating für Hot Chamber berücksichtigt)
  - CC-Leitungen: CC1 und CC2 jeweils mit eigenem 10kOhm Widerstand an VBUS (hinter der Sicherung) gezogen (Source 3A Advertisement)
  - ESD-Schutz: SRV05-4 TVS-Array
- **USB-A Port (Webcam):**
  - Polyfuse: 1.5A Hold Current
- **ESD-Schutz:** SRV05-4 TVS-Array

## 📋 Klipper Konfiguration

### Basis MCU Setup
```ini
[mcu hat]
canbus_uuid: your_uuid_here
# Alternative für USB-Verbindung:
# serial: /dev/serial/by-id/usb-katapult_stm32g0b1xx_your_id-if00

[temperature_sensor hat_mcu]
sensor_type: temperature_mcu
sensor_mcu: hat
```

## 🔌 Pinout & Steckerbelegung

### Stromeingang
- **XT30PW-F (liegend)** oder **5.08mm Schraubklemme**
  - Pin 1: +24V
  - Pin 2: GND

### CAN-Bus Anschlüsse
- **2x Molex Micro-Fit 3.0 (2x2, THT)**
  - Pin 1: +24V (obere Reihe, links)
  - Pin 2: GND (obere Reihe, rechts)
  - Pin 3: CAN_H (untere Reihe, links)
  - Pin 4: CAN_L (untere Reihe, rechts)

### USB Anschlüsse
- **USB-C:** Touchscreen-Verbindung (5V/3A Fähigkeit)
- **USB-A:** Webcam-Verbindung (vertikale Montage)

## 🛠️ PCB Spezifikationen

| Parameter | Spezifikation |
|-----------|---------------|
| **Formfaktor** | Standard HAT (65mm x 56mm) |
| **Lagenaufbau** | 4-Layer Multilayer |
| **Layer 1 (Top)** | Signal/Power |
| **Layer 2** | GND Plane |
| **Layer 3** | 5V Power Plane |
| **Layer 4 (Bottom)** | Signal/GND |
| **Lötstopplack** | Schwarz Matt |
| **Oberfläche** | ENIG (Gold) |
| **Silkscreen** | Weiß |
| **Montage** | Innere Montagelöcher (Pi/Rock kompatibel) |
| **EMV-Design** | Montagelöcher via Stitching-Vias mit GND-Planes verbunden (Layer 2 & Bottom), isoliert von 5V-Plane (Layer 3) |

## ⚠️ Fertigungsempfehlungen

### PCB-Fertigung
- **Kupferdicke:** **2oz Kupfer empfohlen** für verbesserte thermische Leistung und Strombelastbarkeit
- **Via-Spezifikationen:** Minimum 12-20 Vias am DC/DC PGND für Thermal Management
- **Impedanzkontrolle:** 90 Ω Differentiell für USB-Leiterbahnen (D+/D- Paare)

### Bestückungshinweise
- **24V Pfad:** Minimum 3mm Leiterbahnbreite auf Top & Bottom Layern für 12A Gesamtstrom
- **5V Verteilung:** Nutzung der Layer 3 Plane mit massiven Anbindungen (keine Thermals) an Header und Kondensatoren
- **Thermal Management:** QFN GND-Pads mit Vias angebunden, Bottom Layer Routing berücksichtigt

### Bauteilbeschaffung
- Polyfuse-Derating für Kammertemperaturen bis 60°C berücksichtigen
- Induktivitäten mit ausreichendem Sättigungsstrom wählen (>15A empfohlen)
- Low-ESR Kondensatoren für Schaltregler verwenden

## 🛠️ Technische Spezifikationen

| Parameter | Wert | Einheit |
|-----------|------|---------|
| **Eingangsspannung** | 24 ± 2 | V |
| **5V Ausgangsstrom** | 8 | A |
| **Logik-Versorgungsstrom** | 2 | A |
| **CAN-Baudrate** | bis zu 1 | Mbit/s |
| **USB-Geschwindigkeit** | 480 | Mbit/s |
| **Betriebstemperatur** | -10 bis +70 | °C |
| **Abmessungen** | 65 x 56 | mm |

## 📚 Weitere Ressourcen

- [Klipper Dokumentation](https://www.klipper3d.org/Config_Reference.html)
- [CAN-Bus Setup Guide](https://www.klipper3d.org/CANBUS.html)
- [Voron Documentation](https://docs.vorondesign.com/)
- [STM32G0 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0454-stm32g0x0-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [Katapult Firmware Flasher](https://github.com/Arksine/katapult)

## Lizenz

Dieses Projekt steht unter der **Creative Commons Namensnennung-Nicht kommerziell-Share Alike 4.0 International Lizenz** (CC BY-NC-SA 4.0).

[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/deed.de)

Dies bedeutet, Sie dürfen:
- **Teilen** — das Material in jedwedem Format oder Medium vervielfältigen und weiterverbreiten
- **Bearbeiten** — das Material remixen, verändern und darauf aufbauen

Unter folgenden Bedingungen:
- **Namensnennung** — Sie müssen angemessene Urheber- und Rechteangaben machen, einen Link zur Lizenz beifügen und angeben, ob Änderungen vorgenommen wurden
- **Nicht kommerziell** — Sie dürfen das Material nicht für kommerzielle Zwecke nutzen
- **Weitergabe unter gleichen Bedingungen** — Wenn Sie das Material remixen, verändern oder anderweitig direkt darauf aufbauen, dürfen Sie Ihre Beiträge nur unter derselben Lizenz wie das Original verbreiten

Den vollständigen Lizenztext finden Sie unter: https://creativecommons.org/licenses/by-nc-sa/4.0/deed.de

---

> **Hinweis:** Dies ist die Originaldokumentation in deutscher Sprache. Eine englische Übersetzung finden Sie in [README.md](README.md).