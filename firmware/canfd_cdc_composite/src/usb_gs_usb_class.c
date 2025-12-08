/*
 * Minimal USB gs_usb interface - pragmatic approach to avoid deprecated warnings
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include "usb_gs_usb_class.h"

LOG_MODULE_REGISTER(usb_gs_usb, LOG_LEVEL_DBG);

/* Function to send frame to host */
int gs_usb_send_frame_to_host(const uint8_t *data, uint32_t len)
{
    LOG_DBG("Would send %d bytes to host", len);
    return 0;
}

/* Minimal initialization - for now just log that we're ready */
int usb_gs_usb_init(void)
{
    LOG_INF("gs_usb USB interface ready (minimal implementation)");
    
    // For now, we'll implement a minimal version that compiles without warnings
    // Full USB implementation will be added incrementally
    
    return 0;
}