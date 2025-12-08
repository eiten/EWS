/*
 * CandleLight-compatible GS_USB USB Device Descriptors
 * Full compatibility with gs_usb protocol and Linux can-utils
 */

#include <zephyr/usb/usb_device.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/logging/log.h>
#include "usb_gs_descriptors.h"
#include "gs_usb_can.h"

LOG_MODULE_REGISTER(usb_gs_desc, LOG_LEVEL_DBG);

/* CandleLight-compatible device descriptor */
const struct usb_device_descriptor gs_usb_dev_desc = {
    .bLength = USB_DEVICE_DESC_SIZE,
    .bDescriptorType = USB_DESC_DEVICE,
    .bcdUSB = sys_cpu_to_le16(USB_SRN_2_0),
    .bDeviceClass = 0x00,    /* Interface-defined */
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = 64,   /* 64 bytes for control endpoint */
    .idVendor = sys_cpu_to_le16(0x1D50),   /* OpenMoko VID (candleLight standard) */
    .idProduct = sys_cpu_to_le16(0x606F),  /* gs_usb PID (candleLight standard) */
    .bcdDevice = sys_cpu_to_le16(0x0100),  /* Device release 1.0 */
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 3,
    .bNumConfigurations = 1,
};

/* Configuration descriptor */
const struct gs_usb_config_descriptor gs_usb_cfg_desc = {
    .config = {
        .bLength = USB_CFG_DESC_SIZE,
        .bDescriptorType = USB_DESC_CONFIGURATION,
        .wTotalLength = sys_cpu_to_le16(sizeof(struct gs_usb_config_descriptor)),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = USB_SRN_CFG_ATTR_BASE | USB_SRN_CFG_ATTR_SELF_POWERED,
        .bMaxPower = 50, /* 100mA (candleLight compatible) */
    },

    .interface = {
        .bLength = USB_IF_DESC_SIZE,
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 0,  /* Single interface */
        .bAlternateSetting = 0,
        .bNumEndpoints = 2,
        .bInterfaceClass = 0xFF,        /* Vendor specific (candleLight) */
        .bInterfaceSubClass = 0xFF,     /* Vendor specific */
        .bInterfaceProtocol = 0xFF,     /* Vendor specific */
        .iInterface = 0,
    },

    .ep_out = {
        .bLength = USB_EP_DESC_SIZE,
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = GS_USB_EP_OUT,  /* 0x02 */
        .bmAttributes = USB_DC_EP_BULK,
        .wMaxPacketSize = sys_cpu_to_le16(GS_USB_EP_SIZE),  /* 64 bytes */
        .bInterval = 0,
    },

    .ep_in = {
        .bLength = USB_EP_DESC_SIZE,
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = GS_USB_EP_IN,   /* 0x81 */
        .bmAttributes = USB_DC_EP_BULK,
        .wMaxPacketSize = sys_cpu_to_le16(GS_USB_EP_SIZE),  /* 64 bytes */
        .bInterval = 0,
    },
};

/* String descriptors */
static const uint8_t gs_usb_string_lang[] = {
    0x04, 0x03, 0x09, 0x04  /* Language descriptor: English US */
};

static const uint8_t gs_usb_string_manufacturer[] = {
    0x06, 0x03, 'E', 0, 'W', 0, 'S', 0  /* "EWS" */
};

static const uint8_t gs_usb_string_product[] = {
    0x18, 0x03, 
    'E', 0, 'W', 0, 'S', 0, ' ', 0,
    'C', 0, 'A', 0, 'N', 0, '-', 0, 'F', 0, 'D', 0
};

static const uint8_t gs_usb_string_serial[] = {
    0x12, 0x03,
    '0', 0, '0', 0, '0', 0, '0', 0,
    '0', 0, '0', 0, '0', 0, '1', 0
};

const struct usb_string_desription gs_usb_string_desc = {
    .lang_descr = gs_usb_string_lang,
    .utf16le_string = {
        gs_usb_string_manufacturer,
        gs_usb_string_product, 
        gs_usb_string_serial,
    },
};