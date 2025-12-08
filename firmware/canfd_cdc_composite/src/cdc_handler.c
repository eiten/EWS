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

/* Use UART device for PFET control (not CDC over USB) */
static const struct device *cdc_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

#define RX_BUF_SIZE 64
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
    for (int i = 0; i < strlen(response); i++) {
        uart_poll_out(cdc_dev, response[i]);
    }
}

int cdc_handler_init(void)
{
    if (!device_is_ready(cdc_dev)) {
        LOG_ERR("CDC ACM device not ready");
        return -ENODEV;
    }

    LOG_INF("CDC handler initialized");
    return 0;
}

void cdc_handler_process(void)
{
    unsigned char c;
    
    /* Check for incoming characters */
    while (uart_poll_in(cdc_dev, &c) == 0) {
        if (c == '\n' || c == '\r') {
            /* End of command */
            if (rx_pos > 0) {
                rx_buf[rx_pos] = '\0';
                process_command(rx_buf);
                rx_pos = 0;
            }
        } else if (rx_pos < (RX_BUF_SIZE - 1)) {
            /* Add character to buffer */
            rx_buf[rx_pos++] = (char)c;
        } else {
            /* Buffer full, reset */
            rx_pos = 0;
        }
    }
}