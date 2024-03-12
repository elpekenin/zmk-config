/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_gpio

#include <zephyr/device.h>
#include <zephyr/bluetooth/conn.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(elpekenin, CONFIG_ZMK_LOG_LEVEL);

#include <drivers/behavior.h>
#include <dt-bindings/zmk/bt.h>
#include <zmk/behavior.h>

#include "elpekenin/leds.h"

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int on_keymap_binding_pressed(
    struct zmk_behavior_binding *binding,
    struct zmk_behavior_binding_event event
) {
    if (
        // only supported leds
        binding->param1 < 0
        || binding->param1 >= N_LEDS
        // only state off or on
        || binding->param2 < 0
        || binding->param2 > 1
    ) {
        LOG_ERR("Unknown LED number: %d", binding->param1);
        return -ENOTSUP;
    }

    set_led(binding->param1, binding->param2);
    return 0;
}

static int behavior_bt_init(const struct device *dev) { return 0; };

static int on_keymap_binding_released(
    struct zmk_behavior_binding *binding,
    struct zmk_behavior_binding_event event
) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_gpio_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

BEHAVIOR_DT_INST_DEFINE(
    0,
    behavior_bt_init,
    NULL,
    NULL,
    NULL,
    POST_KERNEL,
    CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
    &behavior_gpio_driver_api
);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */