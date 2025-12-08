/*
 * CDC Handler Module  
 * Handles USB CDC ACM commands for PFET control
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "cdc_handler.h"
#include "pfet_control.h"

LOG_MODULE_REGISTER(cdc_handler, LOG_LEVEL_DBG);

#define CDC_DEVICE_NAME "CDC_ACM_0"
#define RX_BUF_SIZE 64

static const struct device *cdc_dev;
static char rx_buf[RX_BUF_SIZE];
static int rx_pos = 0;

static void process_command(char *cmd)
{
    char response[128];
    
    if (strncmp(cmd, "PFET1_ON", 8) == 0) {
        pfet_set_state(1, true);
        strcpy(response, "PFET1 ON\r\n");
    } else if (strncmp(cmd, "PFET1_OFF", 9) == 0) {
        pfet_set_state(1, false);
        strcpy(response, "PFET1 OFF\r\n");
    } else if (strncmp(cmd, "PFET2_ON", 8) == 0) {
        pfet_set_state(2, true);
        strcpy(response, "PFET2 ON\r\n");
    } else if (strncmp(cmd, "PFET2_OFF", 9) == 0) {
        pfet_set_state(2, false);
        strcpy(response, "PFET2 OFF\r\n");
    } else if (strncmp(cmd, "STATUS", 6) == 0) {
        snprintf(response, sizeof(response), 
                "PFET1: %s, PFET2: %s\r\n",
                pfet_get_state(1) ? "ON" : "OFF",
                pfet_get_state(2) ? "ON" : "OFF");
    } else {
        strcpy(response, "ERROR: Unknown command\r\n");
    }
    
    /* Send response */
    uart_poll_out_string(cdc_dev, response);
}

int cdc_handler_init(void)
{
    cdc_dev = device_get_binding(CDC_DEVICE_NAME);
    if (!cdc_dev) {
        LOG_ERR("CDC ACM device not found");
        return -ENODEV;
    }

    LOG_INF("CDC handler initialized");
    return 0;
}

void cdc_handler_process(void)
{
    char c;
    
    /* Check for incoming characters */
    while (uart_poll_in(cdc_dev, (unsigned char *)&c) == 0) {
        if (c == '\n' || c == '\r') {
            /* End of command */
            if (rx_pos > 0) {
                rx_buf[rx_pos] = '\0';
                process_command(rx_buf);
                rx_pos = 0;
            }
        } else if (rx_pos < (RX_BUF_SIZE - 1)) {
            /* Add character to buffer */
            rx_buf[rx_pos++] = c;
        } else {
            /* Buffer full, reset */
            rx_pos = 0;
        }
    }
}