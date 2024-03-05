# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

zephyr_library()
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
zephyr_library_include_directories(include)

file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
zephyr_library_sources(${SRC})
