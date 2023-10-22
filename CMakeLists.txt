cmake_minimum_required(VERSION 3.20.0)

set(BOARD nrf52dk_nrf52832)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(prio-check-demo)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
