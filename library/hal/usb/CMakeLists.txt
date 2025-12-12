add_library(hal_usb
    STATIC usb.c
)

target_link_libraries(hal_usb
	PUBLIC pico_stdlib
	PUBLIC pico_stdio_usb
	PUBLIC hardware_gpio
)

target_include_directories(hal_usb
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)