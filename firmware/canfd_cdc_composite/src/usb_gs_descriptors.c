/*
 * GS_USB USB Device Descriptors Implementation
 */

#include <zephyr/usb/usb_device.h>
#include <zephyr/sys/byteorder.h>
#include "usb_gs_descriptors.h"

/* Device descriptor */
const struct usb_device_descriptor gs_usb_dev_desc = {
    .bLength = USB_DEVICE_DESC_SIZE,
    .bDescriptorType = USB_DESC_DEVICE,
    .bcdUSB = sys_cpu_to_le16(USB_SRN_2_0),
    .bDeviceClass = 0,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = USB_MAX_CTRL_MPS,
    .idVendor = sys_cpu_to_le16(GS_USB_VENDOR_ID),
    .idProduct = sys_cpu_to_le16(GS_USB_PRODUCT_ID),
    .bcdDevice = sys_cpu_to_le16(0x0100), /* Device release 1.0 */
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
        .bMaxPower = 250, /* 500mA */
    },

    .interface = {
        .bLength = USB_IF_DESC_SIZE,
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = GS_USB_INTERFACE_NUM,
        .bAlternateSetting = 0,
        .bNumEndpoints = 2,
        .bInterfaceClass = USB_CLASS_VENDOR_SPECIFIC,
        .bInterfaceSubClass = USB_SUBCLASS_VENDOR,
        .bInterfaceProtocol = USB_PROTOCOL_VENDOR,
        .iInterface = 0,
    },

    .ep_out = {
        .bLength = USB_EP_DESC_SIZE,
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = GS_USB_BULK_EP_OUT,
        .bmAttributes = USB_DC_EP_BULK,
        .wMaxPacketSize = sys_cpu_to_le16(GS_USB_BULK_EP_MPS),
        .bInterval = 0,
    },

    .ep_in = {
        .bLength = USB_EP_DESC_SIZE,
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = GS_USB_BULK_EP_IN,
        .bmAttributes = USB_DC_EP_BULK,
        .wMaxPacketSize = sys_cpu_to_le16(GS_USB_BULK_EP_MPS),
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