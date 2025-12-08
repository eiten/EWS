/*
 * Simple GS_USB USB Interface Implementation
 * Basic gs_usb protocol support integrated with legacy USB stack
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "usb_gs_usb_class.h"
#include "gs_usb_can.h"

LOG_MODULE_REGISTER(usb_gs_usb, LOG_LEVEL_DBG);

/* USB device state */
static bool usb_enabled = false;
static bool can_started = false;

/* Device configuration data */
static struct gs_device_config device_config = {
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
    .icount = 1,
    .sw_version = 0x00010000,
    .hw_version = 0x00010000
};

static struct gs_device_bt_const bt_const = {
    .feature = GS_CAN_FEATURE_FD,
    .fclk_can = 64000000,
    .tseg1_min = 1,
    .tseg1_max = 256,
    .tseg2_min = 1,
    .tseg2_max = 128,
    .sjw_max = 128,
    .brp_min = 1,
    .brp_max = 1024,
    .brp_inc = 1
};

static struct gs_device_bittiming current_bittiming = {
    .prop_seg = 6,
    .phase_seg1 = 7,
    .phase_seg2 = 2,
    .sjw = 1,
    .brp = 10
};

static struct gs_device_bittiming data_bittiming = {
    .prop_seg = 2,
    .phase_seg1 = 3,
    .phase_seg2 = 2,
    .sjw = 1,
    .brp = 5
};

/* Custom vendor request handler for GS_USB protocol */
static int gs_usb_vendor_handler(struct usb_setup_packet *setup, int32_t *len, uint8_t **data)
{
    LOG_DBG("GS_USB vendor request: req=0x%02x, type=0x%02x", setup->bRequest, setup->bmRequestType);

    switch (setup->bRequest) {
    case GS_USB_BREQ_HOST_FORMAT:
        LOG_INF("Host format request");
        return 0;

    case GS_USB_BREQ_BITTIMING:
        if (setup->bmRequestType & USB_EP_DIR_IN) {
            /* GET bittiming */
            *data = (uint8_t *)&current_bittiming;
            *len = sizeof(current_bittiming);
            LOG_INF("Get bittiming");
        } else {
            /* SET bittiming */
            if (*len >= sizeof(current_bittiming)) {
                memcpy(&current_bittiming, *data, sizeof(current_bittiming));
                LOG_INF("Set bittiming");
            }
        }
        return 0;

    case GS_USB_BREQ_MODE:
        if (setup->bmRequestType & USB_EP_DIR_IN) {
            /* GET mode */
            static struct gs_device_mode mode;
            mode.mode = can_started ? 1 : 0;  /* GS_CAN_MODE_START : GS_CAN_MODE_STOP */
            mode.flags = 0;
            *data = (uint8_t *)&mode;
            *len = sizeof(mode);
            LOG_INF("Get mode: %s", can_started ? "START" : "STOP");
        } else {
            /* SET mode */
            if (*len >= sizeof(struct gs_device_mode)) {
                struct gs_device_mode *mode = (struct gs_device_mode *)*data;
                if (mode->mode == 1 && !can_started) {  /* GS_CAN_MODE_START */
                    can_started = true;
                    LOG_INF("Starting CAN");
                    gs_usb_can_start();
                } else if (mode->mode == 0 && can_started) {  /* GS_CAN_MODE_STOP */
                    can_started = false;
                    LOG_INF("Stopping CAN");
                    gs_usb_can_stop();
                }
            }
        }
        return 0;

    case GS_USB_BREQ_BT_CONST:
        if (setup->bmRequestType & USB_EP_DIR_IN) {
            /* GET bit timing constants */
            *data = (uint8_t *)&bt_const;
            *len = sizeof(bt_const);
            LOG_INF("Get bit timing constants");
        }
        return 0;

    case GS_USB_BREQ_DEVICE_CONFIG:
        if (setup->bmRequestType & USB_EP_DIR_IN) {
            /* GET device config */
            *data = (uint8_t *)&device_config;
            *len = sizeof(device_config);
            LOG_INF("Get device config");
        } else {
            /* SET device config */
            if (*len >= sizeof(device_config)) {
                memcpy(&device_config, *data, sizeof(device_config));
                LOG_INF("Set device config");
            }
        }
        return 0;

    case GS_USB_BREQ_TIMESTAMP:
        if (setup->bmRequestType & USB_EP_DIR_IN) {
            /* GET timestamp */
            static uint32_t timestamp;
            timestamp = k_cycle_get_32();
            *data = (uint8_t *)&timestamp;
            *len = sizeof(timestamp);
        }
        return 0;

    case GS_USB_BREQ_DATA_BITTIMING:
        if (setup->bmRequestType & USB_EP_DIR_IN) {
            /* GET data bittiming (FD) */
            *data = (uint8_t *)&data_bittiming;
            *len = sizeof(data_bittiming);
            LOG_INF("Get data bittiming");
        } else {
            /* SET data bittiming (FD) */
            if (*len >= sizeof(data_bittiming)) {
                memcpy(&data_bittiming, *data, sizeof(data_bittiming));
                LOG_INF("Set data bittiming");
            }
        }
        return 0;

    default:
        LOG_WRN("Unknown vendor request: 0x%02x", setup->bRequest);
        return -ENOTSUP;
    }
}

/* Send CAN frame to host via USB */
int gs_usb_send_frame(struct gs_host_frame *frame)
{
    if (!usb_enabled || !can_started) {
        return -ENOTCONN;
    }

    LOG_DBG("Send USB frame: id=0x%08X, dlc=%d", frame->can_id, frame->can_dlc);
    return 0; /* Simplified for now */
}

/* Send frame to host (legacy compatibility) */
int gs_usb_send_frame_to_host(const uint8_t *data, uint32_t len)
{
    if (!usb_enabled) {
        return -ENOTCONN;
    }

    LOG_DBG("Send frame to host: %d bytes", len);
    return 0; /* Simplified for now */
}

/* Initialize GS_USB (for main.c compatibility) */
int usb_gs_usb_init(void)
{
    return gs_usb_class_init();
}

/* Check if GS_USB is ready */
bool gs_usb_is_ready(void)
{
    return usb_enabled && can_started;
}

/* Initialize GS_USB class */
int gs_usb_class_init(void)
{
    LOG_INF("Initializing simple GS_USB class");
    
    /* TODO: Register vendor request handler when needed */
    /* usb_register_request_handler(USB_REQTYPE_TYPE_VENDOR, gs_usb_vendor_handler); */
    
    usb_enabled = true;
    
    LOG_INF("GS_USB class initialized");
    return 0;
}
