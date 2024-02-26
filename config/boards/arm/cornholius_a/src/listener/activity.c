/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>

#include "leds.h"


LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);


static void sleeping_callback(struct k_work *work) {
    // LED on, turn off for 3 seconds
    // LED off, turn on for 1 second
    const bool state = get_led_state(BLUE_LED);
    const uint16_t delay = state ? 3 : 1;
    set_led(BLUE_LED, !state);

    k_work_schedule(k_work_delayable_from_work(work), K_SECONDS(delay));
}

static K_WORK_DELAYABLE_DEFINE(sleep_work, sleeping_callback);


static int activity_change_listener(const zmk_event_t *eh) {
    struct zmk_activity_state_changed *evt = as_zmk_activity_state_changed(eh);

    set_led(GREEN_LED, false);
    set_led(BLUE_LED, false);

    if (evt->state == ZMK_ACTIVITY_IDLE) {
        k_work_schedule(&sleep_work, K_NO_WAIT);
    } else {
        k_work_cancel_delayable(&sleep_work);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(activity_led, activity_change_listener);
ZMK_SUBSCRIPTION(activity_led, zmk_activity_state_changed);
