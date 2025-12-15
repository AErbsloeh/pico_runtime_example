message(STATUS "hal_led available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_led INTERFACE)

target_sources(hal_led INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/led.c
)

target_link_libraries(hal_led INTERFACE
	pico_stdlib
	hardware_gpio
)

target_include_directories(hal_led INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
