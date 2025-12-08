/*
 * PFET Control Module
 * Controls the two output PFETs on the EWS board
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "pfet_control.h"

LOG_MODULE_REGISTER(pfet_control, LOG_LEVEL_DBG);

/* PFET control pin definitions */
static const struct gpio_dt_spec pfet1 = GPIO_DT_SPEC_GET(DT_ALIAS(pfet0), gpios);
static const struct gpio_dt_spec pfet2 = GPIO_DT_SPEC_GET(DT_ALIAS(pfet1), gpios);
static bool pfet1_state = false;
static bool pfet2_state = false;

int pfet_control_init(void)
{
    int ret;

    if (!gpio_is_ready_dt(&pfet1) || !gpio_is_ready_dt(&pfet2)) {
        LOG_ERR("PFET GPIO devices not ready");
        return -ENODEV;
    }

    /* Configure PFET pins as output, initially off */
    ret = gpio_pin_configure_dt(&pfet1, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure PFET1 pin: %d", ret);
        return ret;
    }

    ret = gpio_pin_configure_dt(&pfet2, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure PFET2 pin: %d", ret);
        return ret;
    }

    LOG_INF("PFET control initialized");
    return 0;
}

int pfet_set_state(int pfet_num, bool state)
{
    int ret;
    
    if (pfet_num == 1) {
        ret = gpio_pin_set_dt(&pfet1, state ? 1 : 0);
        if (ret == 0) {
            pfet1_state = state;
            LOG_INF("PFET1 %s", state ? "ON" : "OFF");
        }
    } else if (pfet_num == 2) {
        ret = gpio_pin_set_dt(&pfet2, state ? 1 : 0);
        if (ret == 0) {
            pfet2_state = state;
            LOG_INF("PFET2 %s", state ? "ON" : "OFF");
        }
    } else {
        return -EINVAL;
    }

    return ret;
}

bool pfet_get_state(int pfet_num)
{
    if (pfet_num == 1) {
        return pfet1_state;
    } else if (pfet_num == 2) {
        return pfet2_state;
    }
    return false;
}