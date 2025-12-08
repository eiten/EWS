#ifndef USB_GS_USB_CLASS_H
#define USB_GS_USB_CLASS_H

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usb_ch9.h>

#ifdef __cplusplus
extern "C" {
#endif

/* gs_usb vendor request definitions */
#define GS_USB_VENDOR_REQ_OUT    0x01
#define GS_USB_VENDOR_REQ_IN     0x02

/* gs_usb functions from gs_usb_can.h */
extern int gs_usb_host_frame_received(const uint8_t *data, uint32_t len);
extern int gs_usb_get_frame_to_send(uint8_t *buffer, uint32_t max_len);

/* Function to send frame to host */
extern int gs_usb_send_frame_to_host(const uint8_t *data, uint32_t len);

/* Initialize the gs_usb USB class */
int usb_gs_usb_init(void);

#ifdef __cplusplus
}
#endif

#endif /* USB_GS_USB_CLASS_H */