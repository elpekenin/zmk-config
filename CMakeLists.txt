# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

zephyr_library()

zephyr_library_include_directories(include)  # my files
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)  # zmk files (i think)

# standalone files
zephyr_library_sources(src/elpekenin.c)
# zephyr_library_sources(src/leds.c)

# behaviors
zephyr_library_sources(src/behaviors/behavior_gpio.c)

# events
# zephyr_library_sources(src/event_listeners/activity.c)
# zephyr_library_sources(src/event_listeners/layer.c)
