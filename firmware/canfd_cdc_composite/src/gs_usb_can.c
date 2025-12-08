/*
 * gs_usb CAN Interface
 * Implements gs_usb protocol for CAN FD communication
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/logging/log.h>

#include "gs_usb_can.h"

LOG_MODULE_REGISTER(gs_usb_can, LOG_LEVEL_DBG);

/* CAN device */
static const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

int gs_usb_can_init(void)
{
    int ret;

    if (!device_is_ready(can_dev)) {
        LOG_ERR("CAN device not ready");
        return -ENODEV;
    }

    /* Configure CAN timing for 500kbps (adjust as needed) */
    struct can_timing timing = {
        .sjw = 1,
        .prop_seg = 6,
        .phase_seg1 = 7,
        .phase_seg2 = 2,
        .prescaler = 6
    };

    ret = can_set_timing(can_dev, &timing);
    if (ret < 0) {
        LOG_ERR("Failed to set CAN timing: %d", ret);
        return ret;
    }

    /* Start CAN controller */
    ret = can_start(can_dev);
    if (ret < 0) {
        LOG_ERR("Failed to start CAN: %d", ret);
        return ret;
    }

    LOG_INF("gs_usb CAN interface initialized");
    return 0;
}

int gs_usb_can_send_frame(const struct can_frame *frame)
{
    return can_send(can_dev, frame, K_FOREVER, NULL, NULL);
}

/* TODO: Implement full gs_usb protocol */
/* This would require implementing the USB bulk endpoints and 
   gs_usb command/response protocol as defined in:
   https://github.com/candle-usb/candleLight_fw
*/