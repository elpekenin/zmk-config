/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(elpekenin, CONFIG_ZMK_LOG_LEVEL);

#include <zephyr/sys/__assert.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>

#include "elpekenin/leds.h"


static bool leds_state[N_LEDS] = {false, false};
static const struct gpio_dt_spec leds[N_LEDS] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(green), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(blue), gpios),
};


static int init_leds(void) {
    for (uint8_t pin_no = 0; pin_no < N_LEDS; ++pin_no) {
        const struct gpio_dt_spec *pin_ptr = &leds[pin_no];

        if (
            !gpio_is_ready_dt(pin_ptr)
            || gpio_pin_configure_dt(pin_ptr, GPIO_OUTPUT_ACTIVE) != 0
        ) {
            LOG_ERR("LED did not cooperate: extermine");
            return -ENODEV;
        }

        set_led(pin_no, false);
    }

    return 0;
}

SYS_INIT(init_leds, APPLICATION, 99);


void set_led(uint8_t pin_no, bool state) {
    __ASSERT(pin_no < N_LEDS, "Invalid pin_no: %d", pin_no);

    const struct gpio_dt_spec *ptr = &leds[pin_no];
    gpio_pin_set_dt(ptr, state);
    leds_state[pin_no] = state;
}


bool get_led_state(uint8_t pin_no) {
    __ASSERT(pin_no < N_LEDS, "Invalid pin_no: %d", pin_no);
    return leds_state[pin_no];
}