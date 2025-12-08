/*
 * PFET Control Header
 */

#ifndef PFET_CONTROL_H
#define PFET_CONTROL_H

#include <stdbool.h>

/**
 * Initialize PFET control
 * @return 0 on success, negative error code on failure
 */
int pfet_control_init(void);

/**
 * Set PFET state
 * @param pfet_num PFET number (1 or 2)
 * @param state true for ON, false for OFF
 * @return 0 on success, negative error code on failure
 */
int pfet_set_state(int pfet_num, bool state);

/**
 * Get current PFET state
 * @param pfet_num PFET number (1 or 2)
 * @return current state (true=ON, false=OFF)
 */
bool pfet_get_state(int pfet_num);

#endif /* PFET_CONTROL_H */