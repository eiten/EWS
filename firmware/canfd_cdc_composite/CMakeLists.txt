cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ews_canfd_cdc_composite)

target_sources(app PRIVATE
    src/main.c
    src/cdc_handler.c
    src/pfet_control.c
    src/gs_usb_can.c
)