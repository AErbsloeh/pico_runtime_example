message(STATUS "adc_ad7779 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(adc_ad7779 INTERFACE)

target_sources(adc_ad7779 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad7779.c
)

target_link_libraries(adc_ad7779 INTERFACE
    pico_stdlib
	hardware_gpio
    hardware_spi
    hal_spi
)

target_include_directories(adc_ad7779 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
