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
#include <zmk/behavior_queue.h>

#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>

#include <drivers/behavior.h>

// TODO: Do not hardcode on/off and/or make the yaml check for those names

struct layer_cb_cfg {
    int8_t layer;
    size_t on_count;
    size_t off_count;
    struct zmk_behavior_binding bindings[];
};

#define ON_NODE(n) DT_CHILD(n, on)
#define OFF_NODE(n) DT_CHILD(n, off)

#define _EXTRACT_BINDINGS(n) \
    LISTIFY(DT_PROP_LEN(n, bindings), ZMK_KEYMAP_EXTRACT_BINDING, (, ), n)

#define EXTRACT_BINDINGS(n) \
    { \
        _EXTRACT_BINDINGS(ON_NODE(n)), \
        _EXTRACT_BINDINGS(OFF_NODE(n)) \
    }

#define LAYER_CB_STRUCT(n) \
    static struct layer_cb_cfg callback_##n = { \
        .layer = DT_PROP(n, layer), \
        .on_count = DT_PROP_LEN(ON_NODE(n), bindings), \
        .off_count = DT_PROP_LEN(OFF_NODE(n), bindings), \
        .bindings = EXTRACT_BINDINGS(n) \
    };

DT_INST_FOREACH_CHILD(0, LAYER_CB_STRUCT)

#define LAYER_CB_STRUCT_REF_AND_COMMA(n) \
    &callback_##n

static struct layer_cb_cfg *callbacks[] = {
    DT_INST_FOREACH_CHILD(0, LAYER_CB_STRUCT_REF_AND_COMMA)
};

#define N_CBS ARRAY_SIZE(callbacks)

// ^ Data model
// -----
// v Listener and potentially make actions

static int layer_change_listener(const zmk_event_t *eh) {
    struct zmk_layer_state_changed *evt = as_zmk_layer_state_changed(eh);

    for (size_t i = 0; i < N_CBS; ++i) {
        const struct layer_cb_cfg *callback = callbacks[i];

        if (
            evt->layer != callback->layer
            || (evt->state && !callback->on_count)
            || (!evt->state && !callback->off_count)
        ) {
            continue;
        }

        size_t offset = (evt->state ? 0 : callback->on_count);
        size_t count  = (evt->state ? callback->on_count : callback->off_count);
        for (size_t i = offset; i < (offset + count); ++i) {
            const struct zmk_behavior_binding binding = callback->bindings[i];

            zmk_behavior_queue_add(0, binding, true, 0);  // press
            zmk_behavior_queue_add(0, binding, false, 0); // release
        }
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(layer_callback, layer_change_listener);
ZMK_SUBSCRIPTION(layer_callback, zmk_layer_state_changed);
