add_library(pio_ws2812
    STATIC ws2812.c
)

target_link_libraries(pio_ws2812
    PUBLIC pico_stdlib
	PUBLIC hardware_clocks
    PUBLIC hardware_pio
)

target_include_directories(pio_ws2812
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
