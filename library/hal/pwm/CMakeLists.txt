message(STATUS "hal_pwm available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_pwm INTERFACE)

target_sources(hal_pwm INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/pwm.c
)

target_link_libraries(hal_pwm INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_pwm
)

target_include_directories(hal_pwm INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
