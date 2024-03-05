/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(peke_layer, CONFIG_ZMK_LOG_LEVEL);

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>

#include "leds.h"




static int layer_change_listener(const zmk_event_t *eh) {
    struct zmk_layer_state_changed *evt = as_zmk_layer_state_changed(eh);

    if (evt->layer != 3) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    static bool prev_green, prev_blue;
    if (evt->state) {
        prev_green = get_led_state(GREEN_LED);
        prev_blue = get_led_state(BLUE_LED);

        set_led(GREEN_LED, true);
        set_led(BLUE_LED, false);
    } else {
        set_led(GREEN_LED, prev_green);
        set_led(BLUE_LED, prev_blue);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(layer_led, layer_change_listener);
ZMK_SUBSCRIPTION(layer_led, zmk_layer_state_changed);
