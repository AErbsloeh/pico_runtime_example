message(STATUS "hal_clk available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_clk INTERFACE)

target_sources(hal_clk INTERFACE)

target_link_libraries(hal_clk INTERFACE
    pico_stdlib
    hardware_clocks
    hardware_pio
)

target_include_directories(hal_clk INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
