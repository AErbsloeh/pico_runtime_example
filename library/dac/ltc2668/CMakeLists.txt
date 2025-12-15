message(STATUS "dac_ltc2668 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(dac_ltc2668 INTERFACE)
    
target_sources(dac_ltc2668 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ltc2668.c
)

target_link_libraries(dac_ltc2668 INTERFACE
    pico_stdlib
    hardware_gpio
    hardware_spi
    hal_spi
)

target_include_directories(dac_ltc2668 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
