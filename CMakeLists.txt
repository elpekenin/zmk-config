# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

zephyr_library()

clearly invalid syntax (i think)

zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)

zephyr_library_sources(src/leds.c)

zephyr_library_sources(src/behaviors/behavior_gpio.c)

zephyr_library_sources(src/event_listener/layer.c)
zephyr_library_sources(src/event_listener/activity.c)
