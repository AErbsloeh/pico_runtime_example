message(STATUS "dac_ad5765 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(dac_ad5765 INTERFACE)   

target_sources(dac_ad5765 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad5765.c
)

target_link_libraries(dac_ad5765 INTERFACE     
    pico_stdlib
    hardware_gpio
    hardware_spi
    hal_spi
)

target_include_directories(dac_ad5765 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
