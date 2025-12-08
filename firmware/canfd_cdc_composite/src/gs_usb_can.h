/*
 * gs_usb CAN Interface Header
 * Based on candleLight firmware gs_usb protocol
 */

#ifndef GS_USB_CAN_H
#define GS_USB_CAN_H

#include <zephyr/drivers/can.h>
#include <zephyr/usb/usb_device.h>

/* gs_usb protocol definitions */
#define GS_USB_BREQ_HOST_FORMAT    0
#define GS_USB_BREQ_BITTIMING      1
#define GS_USB_BREQ_MODE           2
#define GS_USB_BREQ_BERR           3
#define GS_USB_BREQ_BT_CONST       4
#define GS_USB_BREQ_DEVICE_CONFIG  5
#define GS_USB_BREQ_TIMESTAMP      6
#define GS_USB_BREQ_IDENTIFY       7

/* Device modes */
#define GS_CAN_MODE_STOP           0
#define GS_CAN_MODE_START          1
#define GS_USB_BREQ_GET_USER_ID    8
#define GS_USB_BREQ_SET_USER_ID    9
#define GS_USB_BREQ_DATA_BITTIMING 10
#define GS_USB_BREQ_BT_CONST_EXT   11
#define GS_USB_BREQ_SET_TERMINATION 12
#define GS_USB_BREQ_GET_TERMINATION 13
#define GS_USB_BREQ_GET_STATE      14

/* gs_usb mode flags */
#define GS_CAN_MODE_NORMAL         0
#define GS_CAN_MODE_LISTEN_ONLY    (1 << 0)
#define GS_CAN_MODE_LOOP_BACK      (1 << 1)
#define GS_CAN_MODE_TRIPLE_SAMPLE  (1 << 2)
#define GS_CAN_MODE_ONE_SHOT       (1 << 3)
#define GS_CAN_MODE_HW_TIMESTAMP   (1 << 4)
#define GS_CAN_MODE_IDENTIFY       (1 << 5)
#define GS_CAN_MODE_USER_ID        (1 << 6)
#define GS_CAN_MODE_PAD_PKTS_TO_MAX_PKT_SIZE (1 << 7)
#define GS_CAN_MODE_FD             (1 << 8)
#define GS_CAN_MODE_REQ_USB_QUIRK_LPC546XX (1 << 9)
#define GS_CAN_MODE_BRS            (1 << 10)

/* gs_usb feature flags */
#define GS_CAN_FEATURE_LISTEN_ONLY       (1 << 0)
#define GS_CAN_FEATURE_LOOP_BACK         (1 << 1)
#define GS_CAN_FEATURE_TRIPLE_SAMPLE     (1 << 2)
#define GS_CAN_FEATURE_ONE_SHOT          (1 << 3)
#define GS_CAN_FEATURE_HW_TIMESTAMP      (1 << 4)
#define GS_CAN_FEATURE_IDENTIFY          (1 << 5)
#define GS_CAN_FEATURE_USER_ID           (1 << 6)
#define GS_CAN_FEATURE_PAD_PKTS_TO_MAX_PKT_SIZE (1 << 7)
#define GS_CAN_FEATURE_FD                (1 << 8)
#define GS_CAN_FEATURE_REQ_USB_QUIRK_LPC546XX (1 << 9)
#define GS_CAN_FEATURE_BRS               (1 << 10)
#define GS_CAN_FEATURE_TERMINATION       (1 << 11)
#define GS_CAN_FEATURE_BERR_REPORTING    (1 << 12)
#define GS_CAN_FEATURE_GET_STATE         (1 << 13)

/* gs_usb frame flags */
#define GS_CAN_FLAG_OVERFLOW            0x01
#define GS_CAN_FLAG_FD                  0x02
#define GS_CAN_FLAG_BRS                 0x04
#define GS_CAN_FLAG_ESI                 0x08
#define GS_CAN_FLAG_ERROR               0x10

/* CAN ID flags (compatible with Linux SocketCAN) */
#define CAN_EFF_FLAG                    0x80000000U  /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG                    0x40000000U  /* Remote transmission request */
#define CAN_ERR_FLAG                    0x20000000U  /* Error frame */
#define CAN_SFF_MASK                    0x000007FFU  /* Standard frame format (SFF) */
#define CAN_EFF_MASK                    0x1FFFFFFFU  /* Extended frame format (EFF) */

struct gs_host_config {
    uint32_t byte_order;
};

struct gs_device_config {
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
    uint8_t icount;
    uint32_t sw_version;
    uint32_t hw_version;
};

struct gs_device_mode {
    uint32_t mode;
    uint32_t flags;
};

struct gs_device_state {
    uint32_t state;
    uint32_t rxerr;
    uint32_t txerr;
};

struct gs_device_bittiming {
    uint32_t prop_seg;
    uint32_t phase_seg1;
    uint32_t phase_seg2;
    uint32_t sjw;
    uint32_t brp;
};

struct gs_device_bt_const {
    uint32_t feature;
    uint32_t fclk_can;
    uint32_t tseg1_min;
    uint32_t tseg1_max;
    uint32_t tseg2_min;
    uint32_t tseg2_max;
    uint32_t sjw_max;
    uint32_t brp_min;
    uint32_t brp_max;
    uint32_t brp_inc;
};

struct gs_host_frame {
    uint32_t echo_id;
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t channel;
    uint8_t flags;
    uint8_t reserved;
    uint8_t data[8];
    uint32_t timestamp_us;
};

/**
 * Initialize gs_usb CAN interface
 * @return 0 on success, negative error code on failure
 */
int gs_usb_can_init(void);

/**
 * Handle gs_usb control requests
 * @param setup USB setup packet
 * @param len Length of data
 * @param data Data buffer
 * @return 0 on success, negative error code on failure
 */
int gs_usb_handle_control_request(struct usb_setup_packet *setup, int32_t *len, uint8_t **data);

/**
 * Send a CAN frame via gs_usb protocol
 * @param frame CAN frame to send
 * @return 0 on success, negative error code on failure
 */
int gs_usb_can_send_frame(const struct can_frame *frame);

/**
 * Process incoming gs_usb frame from host
 * @param gs_frame gs_usb frame from host
 */
void gs_usb_process_host_frame(const struct gs_host_frame *gs_frame);

/**
 * Start CAN interface
 * @return 0 on success, negative error code on failure
 */
int gs_usb_can_start(void);

/**
 * Stop CAN interface
 * @return 0 on success, negative error code on failure  
 */
int gs_usb_can_stop(void);

#endif /* GS_USB_CAN_H */