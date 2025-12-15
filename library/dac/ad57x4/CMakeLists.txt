message(STATUS "dac_ad57x4 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(dac_ad57x4 INTERFACE)

target_sources(dac_ad57x4 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad57x4.c
)

target_link_libraries(dac_ad57x4 INTERFACE
    pico_stdlib
    hardware_gpio
    hardware_spi
	hal_spi
)

target_include_directories(dac_ad57x4 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)