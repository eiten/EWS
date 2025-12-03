# 🛠️ EWS Installation Guide

> 🌍 **Languages:** [English](Installation.en.md) | [Deutsch](Installation.de.md)

A comprehensive guide for installing the Device Tree Overlay and kernel modules for the Universal Voron Power & CAN HAT (EWS).

## 📚 Table of Contents

- [🛠️ EWS Installation Guide](#️-ews-installation-guide)
  - [📚 Table of Contents](#-table-of-contents)
  - [🏔️ Radxa Rock OS](#️-radxa-rock-os)
    - [🔍 Kernel Driver Diagnosis](#-kernel-driver-diagnosis)
    - [📄 Device Tree Setup (DTS)](#-device-tree-setup-dts)
  - [⚠️ Important Notes](#️-important-notes)

## 🏔️ Radxa Rock OS

CAN support in Rock OS varies from version to version. Careful diagnosis is therefore required before installation.

### 🔍 Kernel Driver Diagnosis

#### Step 1: Check Available Modules
First, we check if the required kernel modules are present:

```bash
find /lib/modules/$(uname -r)/kernel/drivers/net/can -name "*.ko*"
```

If the list contains files with names that include `rockchip_canfd` or `m_can`, the modules are present and everything should work.

#### Step 2: Check Kernel Configuration
If no modules were found, we check the kernel configuration:

```bash
grep -E "CONFIG_CAN_ROCKCHIP|CONFIG_CAN_FD" /boot/config-$(uname -r)
```

#### 🚦 Diagnosis Results

##### ✅ Case A: `CONFIG_CAN_ROCKCHIP=y`
- **Status:** ✅ OK
- **Meaning:** The driver is built into the kernel
- **Note:** Does NOT appear under `lsmod` and has no `.ko` file
- **Action:** The overlay is sufficient for activation

##### ⚠️ Case B: `CONFIG_CAN_ROCKCHIP=m`
- **Status:** ⚠️ Check required
- **Meaning:** The driver is a loadable module
- **Check:** Search for module file:
  ```bash
  find /lib/modules/$(uname -r) -name "rockchip_canfd.ko*"
  ```
- **If file missing:** Reinstall kernel packages:
  ```bash
  sudo apt install --reinstall linux-image-rk3588*
  ```

##### ❌ Case C: `# CONFIG_CAN_ROCKCHIP is not set`
- **Status:** ❌ CRITICAL ERROR
- **Symptom:** Output contains `# CONFIG_CAN_ROCKCHIP is not set` and usually also `# CONFIG_CAN_M_CAN is not set`
- **Meaning:** Kernel was compiled without CAN controller support
- **Problem:** The driver is completely missing
- **DKMS Option:** ⚠️ Practically impossible with SoC-internal drivers
- **Solution:** 🔄 Mandatory change of operating system/kernel (e.g. Armbian Vendor Kernel)

### 📄 Device Tree Setup (DTS)

After successful kernel diagnosis, the CAN overlay can be installed:

#### Installation via Radxa Setup Tool

1. **📁 Change Directory**
   ```bash
   cd dts
   ```

2. **🛠️ Start Radxa Setup**
   ```bash
   sudo rsetup
   ```

3. **🧭 Navigation in Setup Menu**
   - Navigate to `Overlays` ➜ `Install 3rd party overlay`

4. **📋 Select Overlay**
   - Choose `rk3588-can1-m1.dts` from the list

5. **🔄 Restart System**
   ```bash
   sudo reboot
   ```

#### ✅ Verify Installation

After reboot, the CAN interface should be available:

```bash
ip link show can1
```

**Expected Output:**
```
3: can1: <NOARP> mtu 72 qdisc noop state DOWN mode DEFAULT group default qlen 10
    link/can
```

## ⚠️ Important Notes

- 🔧 **Jumper Settings:** Ensure all jumpers on the EWS board are set correctly
- 🚨 **Reboot Required:** The system must be restarted after overlay installation
- 📋 **Interface Name:** The CAN interface will be available as `can1` in the system
- 🔌 **Wiring:** Check all connections before first operation
- 🏔️ **OS Compatibility:** This guide specifically refers to Radxa Rock OS

---

> **💡 Tip:** For persistent problems, switching to Armbian with Vendor Kernel is recommended for better CAN support.

> **Note:** The original documentation is in German ([Installation.de.md](Installation.de.md)). This English version is a translation for international users.