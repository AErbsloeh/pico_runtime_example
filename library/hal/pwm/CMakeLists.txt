message(STATUS "hal_pwm available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_pwm
    STATIC pwm.c
)

target_link_libraries(hal_pwm
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_pwm
)

target_include_directories(hal_pwm
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
