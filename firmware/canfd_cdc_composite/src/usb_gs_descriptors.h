/*
 * GS_USB USB Device Descriptors
 * Complete USB device configuration for gs_usb protocol with bulk endpoints
 */

#ifndef USB_GS_DESCRIPTORS_H
#define USB_GS_DESCRIPTORS_H

#include <zephyr/usb/usb_device.h>

/* GS_USB specific USB descriptor values */
#define GS_USB_VENDOR_ID    0x1d50  /* OpenMoko vendor ID */
#define GS_USB_PRODUCT_ID   0x606f  /* gs_usb device ID */

/* Interface and endpoint configuration */
#define GS_USB_INTERFACE_NUM     0
#define GS_USB_BULK_EP_OUT      0x01
#define GS_USB_BULK_EP_IN       0x81
#define GS_USB_BULK_EP_MPS      64

/* USB Class codes for vendor specific interface */
#define USB_CLASS_VENDOR_SPECIFIC   0xFF
#define USB_SUBCLASS_VENDOR         0xFF  
#define USB_PROTOCOL_VENDOR         0xFF

/* USB descriptor structures */
struct gs_usb_config_descriptor {
    struct usb_cfg_descriptor config;
    struct usb_if_descriptor interface;
    struct usb_ep_descriptor ep_out;
    struct usb_ep_descriptor ep_in;
} __packed;

/* Configuration descriptor */
extern const struct gs_usb_config_descriptor gs_usb_cfg_desc;

/* Device descriptor */
extern const struct usb_device_descriptor gs_usb_dev_desc;

/* String descriptors */
extern const struct usb_string_desription gs_usb_string_desc;

#endif /* USB_GS_DESCRIPTORS_H */