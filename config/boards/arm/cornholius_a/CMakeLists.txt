# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

zephyr_library()
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
zephyr_library_include_directories(include)
zephyr_library_sources(src/leds.c)
zephyr_library_sources(src/listener/layer.c)
zephyr_library_sources(src/listener/activity.c)
