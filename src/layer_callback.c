/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT elpekenin_layer_callback

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(elpekenin, CONFIG_ZMK_LOG_LEVEL);

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include <zmk/keymap.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>

#include <drivers/behavior.h>


#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)
// Lets gather all the instances' info
struct layer_cb_cfg {
    int8_t layer;
    uint8_t on_count;
    uint8_t off_count;
    struct zmk_behavior_binding bindings[]; // everything together, ugh
};

#define EXTRACT(n) \
    { \
        LISTIFY(DT_PROP_LEN(n, on_bindings), ZMK_KEYMAP_EXTRACT_BINDING, (, ), n), \
        LISTIFY(DT_PROP_LEN(n, off_bindings), ZMK_KEYMAP_EXTRACT_BINDING, (, ), n) \
    }

#define LAYER_CB_INST_AND_COMMA(n) \
    (static struct layer_cb_cfg) { \
        .layer = DT_PROP(n, layer), \
        .on_count = DT_PROP_LEN(n, on_bindings), \
        .off_count = DT_PROP_LEN(n, off_bindings), \
        .bindings = EXTRACT(n), \
    },

static struct layer_cb_cfg callbacks[] = {
    DT_INST_FOREACH_CHILD(0, LAYER_CB_INST_AND_COMMA)
};

#define N_CBS ARRAY_SIZE(callbacks)

// ^ Data model
// -----
// v Listener and potentially make actions

static int layer_change_listener(const zmk_event_t *eh) {
    struct zmk_layer_state_changed *evt = as_zmk_layer_state_changed(eh);

    for (
        const struct layer_cb_cfg *callback = callbacks;
        callback < &callbacks[N_CBS];
        ++callback
    ) {
        if (evt->layer != callback->layer) {
            continue;
        }

        if (
            (evt->state && !callback->on_count)
            || (!evt->state && callback->off_count)
        ) {
            continue;
        }

        uint8_t offset = (evt->state ? 0 : callback->on_count);
        uint8_t count = (evt->state ? callback->on_count : callback->off_count);
        for (
            const struct zmk_behavior_binding *binding = &callback->bindings[offset];
            binding < &callback->bindings[offset + count];
            binding++
        ) {
            zmk_behavior_queue_add(0, *binding, true, 0);  // press
            zmk_behavior_queue_add(0, *binding, false, 0); // release
        }
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(layer_callback, layer_change_listener);
ZMK_SUBSCRIPTION(layer_callback, zmk_layer_state_changed);

#endif // DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)
