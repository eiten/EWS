# 🛠️ EWS Installation Guide

> 🌍 **Sprachen:** [English](Installation.en.md) | [Deutsch](Installation.de.md)

Ein umfassender Leitfaden zur Installation des Device Tree Overlays und der Kernelmodule für das Universal Voron Power & CAN HAT (EWS).

## 📚 Inhaltsverzeichnis

- [🛠️ EWS Installation Guide](#️-ews-installation-guide)
  - [📚 Inhaltsverzeichnis](#-inhaltsverzeichnis)
  - [🏔️ Radxa Rock OS](#️-radxa-rock-os)
    - [🔍 Kernel-Treiber Diagnose](#-kernel-treiber-diagnose)
    - [📄 Device Tree Setup (DTS)](#-device-tree-setup-dts)
  - [⚠️ Wichtige Hinweise](#️-wichtige-hinweise)

## 🏔️ Radxa Rock OS

Beim Rock OS variiert die CAN-Unterstützung von Version zu Version. Eine sorgfältige Diagnose ist daher vor der Installation erforderlich.

### 🔍 Kernel-Treiber Diagnose

#### Schritt 1: Verfügbare Module prüfen
Zuerst überprüfen wir, ob die erforderlichen Kernelmodule vorhanden sind:

```bash
find /lib/modules/$(uname -r)/kernel/drivers/net/can -name "*.ko*"
```

Sind in der Liste Dateien mit Namen enthalten, die `rockchip_canfd` oder `m_can` enthalten, sind die Module vorhanden und alles sollte funktionieren.

#### Schritt 2: Kernel-Konfiguration überprüfen
Falls keine Module gefunden wurden, prüfen wir die Kernel-Konfiguration:

```bash
grep -E "CONFIG_CAN_ROCKCHIP|CONFIG_CAN_FD" /boot/config-$(uname -r)
```

#### 🚦 Diagnose-Ergebnisse

##### ✅ Fall A: `CONFIG_CAN_ROCKCHIP=y`
- **Status:** ✅ OK
- **Bedeutung:** Der Treiber ist fest im Kernel integriert
- **Hinweis:** Erscheint NICHT unter `lsmod` und hat keine `.ko` Datei
- **Aktion:** Das Overlay reicht zur Aktivierung

##### ⚠️ Fall B: `CONFIG_CAN_ROCKCHIP=m`
- **Status:** ⚠️ Prüfung erforderlich
- **Bedeutung:** Der Treiber ist ein ladbares Modul
- **Prüfung:** Modul-Datei suchen:
  ```bash
  find /lib/modules/$(uname -r) -name "rockchip_canfd.ko*"
  ```
- **Falls Datei fehlt:** Kernel-Pakete reinstallieren:
  ```bash
  sudo apt install --reinstall linux-image-rk3588*
  ```

##### ❌ Fall C: `# CONFIG_CAN_ROCKCHIP is not set`
- **Status:** ❌ KRITISCHER FEHLER
- **Symptom:** Ausgabe enthält `# CONFIG_CAN_ROCKCHIP is not set` und meist auch `# CONFIG_CAN_M_CAN is not set`
- **Bedeutung:** Kernel wurde ohne CAN-Controller-Unterstützung kompiliert
- **Problem:** Der Treiber fehlt komplett
- **DKMS-Option:** ⚠️ Praktisch unmöglich bei SoC-internen Treibern
- **Lösung:** 🔄 Zwingender Wechsel des Betriebssystems/Kernels (z.B. Armbian Vendor Kernel)

### 📄 Device Tree Setup (DTS)

Nach erfolgreicher Kernel-Diagnose kann das CAN-Overlay installiert werden:

#### Installation über Radxa Setup-Tool

1. **📁 Verzeichnis wechseln**
   ```bash
   cd dts
   ```

2. **🛠️ Radxa Setup starten**
   ```bash
   sudo rsetup
   ```

3. **🧭 Navigation im Setup-Menü**
   - Navigiere zu `Overlays` ➜ `Install 3rd party overlay`

4. **📋 Overlay auswählen**
   - Wähle `rk3588-can1-m1.dts` aus der Liste

5. **🔄 System neustarten**
   ```bash
   sudo reboot
   ```

#### ✅ Installation verifizieren

Nach dem Neustart sollte die CAN-Schnittstelle verfügbar sein:

```bash
ip link show can1
```

**Erwartete Ausgabe:**
```
3: can1: <NOARP> mtu 72 qdisc noop state DOWN mode DEFAULT group default qlen 10
    link/can
```

## ⚠️ Wichtige Hinweise

- 🔧 **Jumper-Einstellungen:** Stelle sicher, dass alle Jumper auf dem EWS-Board korrekt gesetzt sind
- 🚨 **Neustart erforderlich:** Das System muss nach der Overlay-Installation neu gestartet werden
- 📋 **Interface-Name:** Die CAN-Schnittstelle wird als `can1` im System verfügbar sein
- 🔌 **Verkabelung:** Überprüfe alle Verbindungen vor der ersten Inbetriebnahme
- 🏔️ **OS-Kompatibilität:** Diese Anleitung bezieht sich speziell auf Radxa Rock OS

---

> **💡 Tipp:** Bei persistenten Problemen empfiehlt sich der Wechsel zu Armbian mit Vendor Kernel für bessere CAN-Unterstützung.

> **Hinweis:** Dies ist die Originaldokumentation in deutscher Sprache. Eine englische Übersetzung finden Sie in [Installation.en.md](Installation.en.md).