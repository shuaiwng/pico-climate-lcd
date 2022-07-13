cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(my_project)

pico_sdk_init()

add_executable(main main.c dht.c lcd_1602_i2c.c
    )

target_link_libraries(main pico_stdlib hardware_gpio hardware_i2c)

pico_add_extra_outputs(main)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
