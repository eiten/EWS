/*
 * CDC Handler Header
 */

#ifndef CDC_HANDLER_H
#define CDC_HANDLER_H

/**
 * Initialize CDC handler
 * @return 0 on success, negative error code on failure
 */
int cdc_handler_init(void);

/**
 * Process incoming CDC commands
 * Should be called regularly from main loop
 */
void cdc_handler_process(void);

#endif /* CDC_HANDLER_H */