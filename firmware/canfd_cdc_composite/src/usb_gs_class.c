/*
 * GS_USB Device Implementation with Bulk Endpoints
 * Complete gs_usb USB device for CAN interface
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/ring_buffer.h>

#include "gs_usb_can.h"
#include "usb_gs_descriptors.h"

LOG_MODULE_REGISTER(usb_gs_device, LOG_LEVEL_DBG);

/* USB endpoint buffers */
#define GS_USB_BUFFER_SIZE 1024
static uint8_t ep_out_buffer[GS_USB_BULK_EP_MPS];
static uint8_t ep_in_buffer[GS_USB_BULK_EP_MPS];

/* Ring buffer for outgoing CAN frames */
RING_BUF_DECLARE(gs_tx_ringbuf, GS_USB_BUFFER_SIZE);

/* Endpoint callbacks */
static void gs_usb_ep_out_cb(uint8_t ep, enum usb_dc_ep_cb_status_code cb_status)
{
    uint32_t bytes_read = 0;
    int ret;
    
    LOG_DBG("EP OUT callback: ep=0x%02x status=%d", ep, cb_status);
    
    if (cb_status == USB_DC_EP_DATA_OUT) {
        /* Read data from OUT endpoint */
        ret = usb_read(ep, ep_out_buffer, sizeof(ep_out_buffer), &bytes_read);
        if (ret == 0 && bytes_read >= sizeof(struct gs_host_frame)) {
            
            /* Process each frame in the buffer */
            uint32_t offset = 0;
            while (offset + sizeof(struct gs_host_frame) <= bytes_read) {
                struct gs_host_frame *frame = (struct gs_host_frame *)(ep_out_buffer + offset);
                
                /* Process the host frame */
                gs_usb_process_host_frame(frame);
                
                offset += sizeof(struct gs_host_frame);
            }
        }
    }
}

static void gs_usb_ep_in_cb(uint8_t ep, enum usb_dc_ep_cb_status_code cb_status)
{
    LOG_DBG("EP IN callback: ep=0x%02x status=%d", ep, cb_status);
    
    if (cb_status == USB_DC_EP_DATA_IN) {
        /* Check if we have more data to send */
        uint32_t available = ring_buf_size_get(&gs_tx_ringbuf);
        if (available >= sizeof(struct gs_host_frame)) {
            /* Send next frame */
            uint32_t bytes_read = ring_buf_get(&gs_tx_ringbuf, ep_in_buffer, sizeof(struct gs_host_frame));
            if (bytes_read == sizeof(struct gs_host_frame)) {
                usb_write(ep, ep_in_buffer, bytes_read, NULL);
            }
        }
    }
}

/* USB device configuration */
static struct usb_ep_cfg_data gs_usb_ep_cfg[] = {
    {
        .ep_cb = gs_usb_ep_out_cb,
        .ep_addr = GS_USB_BULK_EP_OUT,
    },
    {
        .ep_cb = gs_usb_ep_in_cb,  
        .ep_addr = GS_USB_BULK_EP_IN,
    },
};

static struct usb_interface_cfg_data gs_usb_if_cfg = {
    .class = USB_CLASS_VENDOR_SPECIFIC,
    .subclass = USB_SUBCLASS_VENDOR,
    .protocol = USB_PROTOCOL_VENDOR,
    .ep = gs_usb_ep_cfg,
    .num_ep = ARRAY_SIZE(gs_usb_ep_cfg),
};

/* Custom request handler */
static int gs_usb_custom_req_handler(struct usb_setup_packet *setup, int32_t *len, uint8_t **data)
{
    /* Handle vendor-specific control requests */
    if ((setup->bmRequestType & 0x60) == 0x40) {  /* Vendor request */
        return gs_usb_handle_control_request(setup, len, data);
    }
    
    return -ENOTSUP;
}

/* USB configuration structure */
static struct usb_cfg_data gs_usb_cfg = {
    .usb_device_description = &gs_usb_dev_desc,
    .interface_config = gs_usb_custom_req_handler,
    .interface_descriptor = &gs_usb_cfg_desc.interface,
    .cb_usb_status = NULL,
    .interface = {
        .class_handler = NULL,
        .custom_handler = gs_usb_custom_req_handler,
        .vendor_handler = gs_usb_custom_req_handler,
    },
    .num_endpoints = ARRAY_SIZE(gs_usb_ep_cfg),
    .endpoint = gs_usb_ep_cfg,
};

/* Send a gs_usb frame to host */
int gs_usb_send_frame_to_host(const struct gs_host_frame *frame)
{
    uint32_t bytes_written;
    
    /* Try to put frame in ring buffer */
    bytes_written = ring_buf_put(&gs_tx_ringbuf, (uint8_t *)frame, sizeof(struct gs_host_frame));
    
    if (bytes_written != sizeof(struct gs_host_frame)) {
        LOG_WRN("Ring buffer full, dropping frame");
        return -ENOMEM;
    }
    
    /* If this is the first frame in buffer, start transmission */
    if (ring_buf_size_get(&gs_tx_ringbuf) == sizeof(struct gs_host_frame)) {
        uint32_t bytes_read = ring_buf_get(&gs_tx_ringbuf, ep_in_buffer, sizeof(struct gs_host_frame));
        if (bytes_read == sizeof(struct gs_host_frame)) {
            return usb_write(GS_USB_BULK_EP_IN, ep_in_buffer, bytes_read, NULL);
        }
    }
    
    return 0;
}

int usb_gs_class_init(void)
{
    int ret;
    
    /* Configure USB device with our descriptors */
    ret = usb_set_config(&gs_usb_cfg);
    if (ret < 0) {
        LOG_ERR("Failed to configure USB device: %d", ret);
        return ret;
    }
    
    LOG_INF("GS_USB device configured with bulk endpoints");
    return 0;
}