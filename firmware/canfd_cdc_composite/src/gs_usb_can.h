/*
 * gs_usb CAN Interface Header
 */

#ifndef GS_USB_CAN_H
#define GS_USB_CAN_H

#include <zephyr/drivers/can.h>

/**
 * Initialize gs_usb CAN interface
 * @return 0 on success, negative error code on failure
 */
int gs_usb_can_init(void);

/**
 * Send a CAN frame
 * @param frame CAN frame to send
 * @return 0 on success, negative error code on failure
 */
int gs_usb_can_send_frame(const struct can_frame *frame);

#endif /* GS_USB_CAN_H */