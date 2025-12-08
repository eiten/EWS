/*
 * gs_usb CAN Interface Implementation
 * Implements gs_usb protocol for CAN FD communication compatible with candleLight
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "gs_usb_can.h"
#include "usb_gs_usb_class.h"

LOG_MODULE_REGISTER(gs_usb_can, LOG_LEVEL_DBG);

/* CAN device */
static const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

/* gs_usb state */
static struct gs_device_config device_config = {
    .reserved1 = 0,
    .reserved2 = 0, 
    .reserved3 = 0,
    .icount = 1,  /* One CAN interface */
    .sw_version = 0x00010001,
    .hw_version = 0x00010001,
};

static struct gs_device_bt_const bt_const = {
    .feature = GS_CAN_FEATURE_LISTEN_ONLY | 
               GS_CAN_FEATURE_LOOP_BACK |
               GS_CAN_FEATURE_ONE_SHOT |
               GS_CAN_FEATURE_HW_TIMESTAMP |
               GS_CAN_FEATURE_FD |
               GS_CAN_FEATURE_BRS,
    .fclk_can = 64000000,  /* 64MHz CAN clock */
    .tseg1_min = 1,
    .tseg1_max = 256,
    .tseg2_min = 1, 
    .tseg2_max = 128,
    .sjw_max = 128,
    .brp_min = 1,
    .brp_max = 1024,
    .brp_inc = 1,
};

static struct gs_device_mode current_mode = {
    .mode = GS_CAN_MODE_NORMAL,
    .flags = 0,
};

static struct gs_device_state current_state = {
    .state = CAN_STATE_ERROR_ACTIVE,
    .rxerr = 0,
    .txerr = 0,
};

static bool can_started = false;
static uint32_t echo_id_counter = 0;

/* CAN RX callback */
static void can_rx_callback(const struct device *dev, struct can_frame *frame, void *user_data)
{
    struct gs_host_frame gs_frame = {0};
    
    /* Convert CAN frame to gs_usb frame */
    gs_frame.echo_id = 0xFFFFFFFF;  /* RX frame marker */
    gs_frame.can_id = frame->id;
    
    if (frame->flags & CAN_FRAME_IDE) {
        gs_frame.can_id |= CAN_EFF_FLAG;  /* Extended ID flag */
    }
    if (frame->flags & CAN_FRAME_RTR) {
        gs_frame.can_id |= CAN_RTR_FLAG;  /* RTR flag */
    }
    
    /* Handle CAN-FD specific flags */
    if (frame->flags & CAN_FRAME_FDF) {
        gs_frame.flags |= GS_CAN_FLAG_FD;  /* FD frame */
    }
    if (frame->flags & CAN_FRAME_BRS) {
        gs_frame.flags |= GS_CAN_FLAG_BRS;  /* BRS frame */
    }
    if (frame->flags & CAN_FRAME_ESI) {
        gs_frame.flags |= GS_CAN_FLAG_ESI;  /* ESI frame */
    }
    
    gs_frame.can_dlc = frame->dlc;
    gs_frame.channel = 0;
    gs_frame.timestamp_us = k_uptime_get_32() * 1000;  /* Convert to microseconds */
    
    memcpy(gs_frame.data, frame->data, MIN(frame->dlc, 8));
    
    /* Send frame to host via USB bulk IN endpoint */
    gs_usb_send_frame_to_host((const uint8_t*)&gs_frame, sizeof(gs_frame));
    
    LOG_DBG("RX: ID=0x%08x DLC=%d FD=%d", frame->id, frame->dlc, 
            (frame->flags & CAN_FRAME_FDF) ? 1 : 0);
}

int gs_usb_can_init(void)
{
    int ret;

    if (!device_is_ready(can_dev)) {
        LOG_ERR("CAN device not ready");
        return -ENODEV;
    }

    /* Set default timing for 500kbps */
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

    /* Install RX callback */
    struct can_filter filter = {
        .id = 0,
        .mask = 0,
        .flags = CAN_FILTER_IDE,
    };
    
    ret = can_add_rx_filter(can_dev, can_rx_callback, NULL, &filter);
    if (ret < 0) {
        LOG_ERR("Failed to add CAN RX filter: %d", ret);
        return ret;
    }

    LOG_INF("gs_usb CAN interface initialized");
    return 0;
}

int gs_usb_handle_control_request(struct usb_setup_packet *setup, int32_t *len, uint8_t **data)
{
    static uint8_t response_buf[64];
    int ret = 0;

    LOG_DBG("Control request: bRequest=0x%02x wValue=0x%04x wIndex=0x%04x wLength=%d",
            setup->bRequest, setup->wValue, setup->wIndex, setup->wLength);

    switch (setup->bRequest) {
    case GS_USB_BREQ_HOST_FORMAT:
        /* Host format - not much to do */
        *len = 0;
        break;

    case GS_USB_BREQ_DEVICE_CONFIG:
        memcpy(response_buf, &device_config, sizeof(device_config));
        *data = response_buf;
        *len = sizeof(device_config);
        break;

    case GS_USB_BREQ_BT_CONST:
        memcpy(response_buf, &bt_const, sizeof(bt_const));
        *data = response_buf;
        *len = sizeof(bt_const);
        break;

    case GS_USB_BREQ_BITTIMING:
        if (setup->bmRequestType & USB_REQTYPE_DIR_TO_DEVICE) {
            /* Set bit timing */
            struct gs_device_bittiming *bt = (struct gs_device_bittiming *)*data;
            struct can_timing timing = {
                .sjw = bt->sjw,
                .prop_seg = bt->prop_seg,
                .phase_seg1 = bt->phase_seg1,
                .phase_seg2 = bt->phase_seg2,
                .prescaler = bt->brp,
            };
            
            if (can_started) {
                can_stop(can_dev);
                can_started = false;
            }
            
            ret = can_set_timing(can_dev, &timing);
            if (ret < 0) {
                LOG_ERR("Failed to set timing: %d", ret);
                return ret;
            }
            LOG_INF("Set timing: brp=%d sjw=%d prop=%d ph1=%d ph2=%d", 
                    bt->brp, bt->sjw, bt->prop_seg, bt->phase_seg1, bt->phase_seg2);
            *len = 0;
        } else {
            /* Get current bit timing */
            *len = 0; /* TODO: Return current timing */
        }
        break;

    case GS_USB_BREQ_MODE:
        if (setup->bmRequestType & USB_REQTYPE_DIR_TO_DEVICE) {
            /* Set mode */
            struct gs_device_mode *mode = (struct gs_device_mode *)*data;
            current_mode = *mode;
            
            if (mode->mode & GS_CAN_MODE_NORMAL) {
                if (!can_started) {
                    ret = can_start(can_dev);
                    if (ret < 0) {
                        LOG_ERR("Failed to start CAN: %d", ret);
                        return ret;
                    }
                    can_started = true;
                    LOG_INF("CAN started");
                }
            } else {
                if (can_started) {
                    can_stop(can_dev);
                    can_started = false;
                    LOG_INF("CAN stopped");
                }
            }
            *len = 0;
        } else {
            /* Get current mode */
            memcpy(response_buf, &current_mode, sizeof(current_mode));
            *data = response_buf;
            *len = sizeof(current_mode);
        }
        break;

    case GS_USB_BREQ_GET_STATE:
        /* Update error counters - simplified for now */
        current_state.state = CAN_STATE_ERROR_ACTIVE;
        current_state.rxerr = 0;
        current_state.txerr = 0;
        
        memcpy(response_buf, &current_state, sizeof(current_state));
        *data = response_buf;
        *len = sizeof(current_state);
        break;

    case GS_USB_BREQ_IDENTIFY:
        /* LED identify - toggle status LED */
        *len = 0;
        break;

    default:
        LOG_WRN("Unhandled control request: 0x%02x", setup->bRequest);
        ret = -ENOTSUP;
        break;
    }

    return ret;
}

int gs_usb_can_send_frame(const struct can_frame *frame)
{
    if (!can_started) {
        return -ENETDOWN;
    }

    return can_send(can_dev, frame, K_FOREVER, NULL, NULL);
}

/* Handle incoming gs_usb frames from host */
int gs_usb_process_host_frame(const struct gs_host_frame *gs_frame)
{
    struct can_frame frame = {0};
    int ret;

    /* Convert gs_usb frame to CAN frame */
    frame.id = gs_frame->can_id & CAN_EFF_MASK;
    
    if (gs_frame->can_id & CAN_EFF_FLAG) {
        frame.flags |= CAN_FRAME_IDE;  /* Extended ID */
    }
    if (gs_frame->can_id & CAN_RTR_FLAG) {
        frame.flags |= CAN_FRAME_RTR;  /* RTR frame */
    }
    
    /* Handle CAN-FD flags */
    if (gs_frame->flags & GS_CAN_FLAG_FD) {
        frame.flags |= CAN_FRAME_FDF;  /* FD frame */
    }
    if (gs_frame->flags & GS_CAN_FLAG_BRS) {
        frame.flags |= CAN_FRAME_BRS;  /* Bit rate switch */
    }
    if (gs_frame->flags & GS_CAN_FLAG_ESI) {
        frame.flags |= CAN_FRAME_ESI;  /* Error state indicator */
    }

    frame.dlc = gs_frame->can_dlc;
    memcpy(frame.data, gs_frame->data, MIN(frame.dlc, sizeof(frame.data)));

    /* Send frame */
    ret = gs_usb_can_send_frame(&frame);
    
    if (ret == 0) {
        /* Echo frame back to host */
        struct gs_host_frame echo_frame = *gs_frame;
        echo_frame.timestamp_us = k_uptime_get_32() * 1000;  /* Convert to microseconds */
        
        /* Send echo frame via USB bulk IN endpoint */
        gs_usb_send_frame_to_host((const uint8_t*)&echo_frame, sizeof(echo_frame));
        
        LOG_DBG("TX: ID=0x%08x DLC=%d FD=%d", 
                frame.id, frame.dlc, (frame.flags & CAN_FRAME_FDF) ? 1 : 0);
    } else {
        LOG_ERR("Failed to send CAN frame: %d", ret);
        
        /* Send error frame back to host */
        struct gs_host_frame error_frame = *gs_frame;
        error_frame.flags |= GS_CAN_FLAG_ERROR;
        gs_usb_send_frame_to_host((const uint8_t*)&error_frame, sizeof(error_frame));
    }

    return ret;
}

/**
 * Start CAN interface
 */
int gs_usb_can_start(void)
{
    int ret = can_start(can_dev);
    if (ret != 0) {
        LOG_ERR("Failed to start CAN interface: %d", ret);
    } else {
        LOG_INF("CAN interface started");
    }
    return ret;
}

/**
 * Stop CAN interface 
 */
int gs_usb_can_stop(void)
{
    int ret = can_stop(can_dev);
    if (ret != 0) {
        LOG_ERR("Failed to stop CAN interface: %d", ret);
    } else {
        LOG_INF("CAN interface stopped");
    }
    return ret;
}