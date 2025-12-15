message(STATUS "adc_ads8881 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(adc_ads8881 INTERFACE)

target_sources(adc_ads8881 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ads8881.c
)

target_link_libraries(adc_ads8881 INTERFACE
	hardware_gpio
    hardware_spi
    hal_spi
)

target_include_directories(adc_ads8881 INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
