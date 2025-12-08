/*
 * Minimal USB gs_usb interface - just USB enabling for now
 */

#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/logging/log.h>

#include "usb_gs_usb_class.h"

LOG_MODULE_REGISTER(usb_gs_usb, LOG_LEVEL_DBG);

/* Function to send frame to host - stub for now */
int gs_usb_send_frame_to_host(const uint8_t *data, uint32_t len)
{
    /* For now, just log that we would send data */
    LOG_DBG("Would send %d bytes to host", len);
    return 0;
}

/* Initialize gs_usb USB interface - minimal version */
int usb_gs_usb_init(void)
{
    int ret;

    LOG_INF("Initializing minimal gs_usb USB interface");

    /* Just enable basic USB device stack for now */
    ret = usb_enable(NULL);
    if (ret != 0) {
        LOG_ERR("Failed to enable USB: %d", ret);
        return ret;
    }

    LOG_INF("Basic USB interface initialized");
    return 0;
}