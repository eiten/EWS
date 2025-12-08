/*
 * EWS CAN FD CDC Composite Firmware
 * Main application entry point
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/logging/log.h>

#include "cdc_handler.h"
#include "pfet_control.h"
#include "gs_usb_can.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/* LED definitions - adjust to actual EWS board pins */
#define LED_STATUS_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led_status = GPIO_DT_SPEC_GET(LED_STATUS_NODE, gpios);

int main(void)
{
    int ret;

    LOG_INF("EWS CAN FD CDC Composite Firmware starting...");

    /* Initialize status LED */
    if (!gpio_is_ready_dt(&led_status)) {
        LOG_ERR("Status LED device not ready");
        return -ENODEV;
    }
    
    ret = gpio_pin_configure_dt(&led_status, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Error configuring status LED: %d", ret);
        return ret;
    }

    /* Initialize PFET control */
    ret = pfet_control_init();
    if (ret < 0) {
        LOG_ERR("Failed to initialize PFET control: %d", ret);
        return ret;
    }

    /* Initialize USB composite device */
    ret = usb_enable(NULL);
    if (ret != 0) {
        LOG_ERR("Failed to enable USB: %d", ret);
        return ret;
    }

    /* Initialize CDC handler */
    ret = cdc_handler_init();
    if (ret < 0) {
        LOG_ERR("Failed to initialize CDC handler: %d", ret);
        return ret;
    }

    /* Initialize gs_usb CAN interface */
    ret = gs_usb_can_init();
    if (ret < 0) {
        LOG_ERR("Failed to initialize gs_usb CAN: %d", ret);
        return ret;
    }

    LOG_INF("EWS firmware initialized successfully");

    /* Main loop */
    while (1) {
        /* Toggle status LED to show activity */
        gpio_pin_toggle_dt(&led_status);
        
        /* Handle CDC commands */
        cdc_handler_process();
        
        k_msleep(500);
    }

    return 0;
}