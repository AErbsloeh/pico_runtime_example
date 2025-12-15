add_library(eth_ch9121  INTERFACE)

target_sources(eth_ch9121   INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/eth_ch9121.c
)

target_link_libraries(eth_ch9121 INTERFACE
    pico_stdlib
    hardware_clocks
    hardware_pio
    hal_uart
)

target_include_directories(eth_ch9121 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
