# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

zephyr_library()

zephyr_library_include_directories(include)
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)

# standalone files
zephyr_library_sources(src/elpekenin.c)

# behaviors
zephyr_library_sources(src/behaviors/behavior_gpio.c)

# bindings
zephyr_library_sources(src/layer_callback.c)
