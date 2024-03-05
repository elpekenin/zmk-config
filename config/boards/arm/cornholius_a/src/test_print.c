/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(peke_test_print, CONFIG_ZMK_LOG_LEVEL);

#include <stdint.h>

#include <zephyr/init.h>
#include <zephyr/kernel.h>


static int test_suite(void) {
    k_sleep(K_SECONDS(15));

    char text[6] = {};
    uint8_t u8 = 42;
    sprintf(text, " %i", u8);
    LOG_DBG("snprintf'ed u8 with %%i: %s", text);
    LOG_DBG("%%i with u8: %i", u8);

    snprintf(text, sizeof(text), "%d", u8);
    LOG_DBG("snprintf'ed u8 with %%d: %s", text);

    snprintf(text, sizeof(text), "%3u%%", u8);
    LOG_DBG("snprintf'ed u8 with %%3u%%%%: %s", text);


    LOG_DBG("%%s with literal: %s", "Hello world!");
    LOG_DBG("Escaped quote: \"");

    uint16_t u16 = 0xF00;
    LOG_DBG("%%u with u16: %u", u16);

    LOG_DBG("%%x with u16: 0x%x", u16);
    LOG_DBG("%%X with u16: 0x%X", u16);
    LOG_DBG("%%02x with u16: 0x%02x", u16);
    LOG_DBG("%%02X with u16: 0x%02X", u16);
    LOG_DBG("%%04x with u16: 0x%04x", u16);
    LOG_DBG("%%04X with u16: 0x%04X", u16);

    LOG_DBG("%%p with NULL: %p", NULL);
    LOG_DBG("%%p with text: %p", text);

    return 0;
}

SYS_INIT(test_suite, APPLICATION, 99);
