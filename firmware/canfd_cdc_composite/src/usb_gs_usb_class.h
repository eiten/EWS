#ifndef USB_GS_USB_CLASS_H
#define USB_GS_USB_CLASS_H

#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usb_ch9.h>
#include "gs_usb_can.h"

#ifdef __cplusplus
extern "C" {
#endif

/* USB gs_usb class specific constants */
#define GS_USB_VENDOR_ID           0x1d50
#define GS_USB_PRODUCT_ID          0x606f
#define GS_USB_INTERFACE_CLASS     0xff  /* Vendor specific */
#define GS_USB_INTERFACE_SUBCLASS  0x00
#define GS_USB_INTERFACE_PROTOCOL  0x00

/* USB endpoint addresses */
#define GS_USB_EP_OUT              0x01
#define GS_USB_EP_IN               0x81

/* USB endpoint sizes */
#define GS_USB_EP_MPS              64

/* USB string indices */
#define GS_USB_STRING_MANUFACTURER 1
#define GS_USB_STRING_PRODUCT      2
#define GS_USB_STRING_SERIAL       3

/**
 * Initialize USB gs_usb class
 * @return 0 on success, negative error code on failure
 */
int usb_gs_usb_init(void);

/**
 * Send gs_usb frame to host via USB
 * @param frame gs_usb frame to send
 * @return 0 on success, negative error code on failure
 */
int usb_gs_usb_send_frame(const struct gs_host_frame *frame);

/**
 * Function to send frame to host (legacy compatibility)
 * @param data Raw data to send
 * @param len Length of data
 * @return 0 on success, negative error code on failure
 */
int gs_usb_send_frame_to_host(const uint8_t *data, uint32_t len);

/**
 * Initialize GS_USB (main.c compatibility)
 * @return 0 on success, negative error code on failure
 */
int usb_gs_usb_init(void);

#ifdef __cplusplus
}
#endif

#endif /* USB_GS_USB_CLASS_H */