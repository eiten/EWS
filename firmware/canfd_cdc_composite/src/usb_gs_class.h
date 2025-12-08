/*
 * USB gs_usb Class Header
 */

#ifndef USB_GS_CLASS_H
#define USB_GS_CLASS_H

#include "gs_usb_can.h"

/**
 * Initialize USB gs_usb class
 * @return 0 on success, negative error code on failure
 */
int usb_gs_class_init(void);

/**
 * Send a gs_usb frame to host via USB bulk IN endpoint
 * @param frame gs_usb frame to send
 * @return 0 on success, negative error code on failure
 */
int gs_usb_send_frame_to_host(const struct gs_host_frame *frame);

#endif /* USB_GS_CLASS_H */