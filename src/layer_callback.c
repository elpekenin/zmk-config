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


#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)
// TODO: Make array flexible
//       Probably needs iterating all the nodes and creating standalone ...
//       ... structs and then reference (&) them
struct action {
    uint8_t count;
    struct zmk_behavior_binding bindings[30];
};

struct layer_cb_cfg {
    int8_t layer;
    struct action on;
    struct action off;
};

#define EXTRACT(n) \
    {LISTIFY(DT_PROP_LEN(n, bindings), ZMK_KEYMAP_EXTRACT_BINDING, (, ), n)}

#define EXTRACT_BINDINGS(n) \
    { \
        .count = DT_PROP_LEN(n, bindings), \
        .bindings = EXTRACT(n) \
    }

#define LAYER_CB_INST_AND_COMMA(n) \
    { \
        .layer = DT_PROP(n, layer), \
        .on = EXTRACT_BINDINGS(DT_CHILD(n, on)), \
        .off = EXTRACT_BINDINGS(DT_CHILD(n, off)) \
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
        if (
            evt->layer != callback->layer
            || (evt->state && !callback->on.count)
            || (!evt->state && !callback->off.count)
        ) {
            continue;
        }

        LOG_DBG("Running for layer %d: %d", evt->layer, evt->state);
        struct action action = (evt->state ? callback->on : callback->off);
        for (
            const struct zmk_behavior_binding *binding = action.bindings;
            binding < &action.bindings[action.count];
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
