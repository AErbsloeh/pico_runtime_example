message(STATUS "pio_ws2812 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(peri_ws2812 INTERFACE)

target_sources(peri_ws2812 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ws2812.c
)

target_link_libraries(peri_ws2812 INTERFACE
    pico_stdlib
	hardware_clocks
    hardware_pio
)

target_include_directories(peri_ws2812 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
