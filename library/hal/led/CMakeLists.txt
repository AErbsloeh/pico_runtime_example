add_library(hal_led
    STATIC led.c
)

target_link_libraries(hal_led
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
)

target_include_directories(hal_led
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
