message(STATUS "pio_clock available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(pio_clock
)

target_link_libraries(pio_clock
    PUBLIC pico_stdlib
    PUBLIC hardware_clocks
    PUBLIC hardware_pio
)

target_include_directories(pio_clock
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
