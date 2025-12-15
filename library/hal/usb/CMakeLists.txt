message(STATUS "hal_usb available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_usb INTERFACE)

target_sources(hal_usb INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/usb.c
)

target_link_libraries(hal_usb INTERFACE
	pico_stdlib
	pico_stdio_usb
	hardware_gpio
)

target_include_directories(hal_usb INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)