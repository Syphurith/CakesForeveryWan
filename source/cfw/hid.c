#include "hid.h"

#include <stdint.h>

const volatile uint16_t *hid_regs = (volatile uint16_t *)0x10146000;

uint16_t wait_key(void) {
    uint16_t old_status = *hid_regs;
    uint16_t new_status;
    while ((new_status = *hid_regs) == old_status);

    if (new_status > old_status) {
        return (new_status ^ old_status) | key_released;
    } else {
        return (new_status ^ old_status) | key_pressed;
    }
}

uint16_t get_key(void) {
    return *hid_regs;
}
