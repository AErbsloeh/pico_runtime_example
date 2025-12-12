message(STATUS "adc_ads8881 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(adc_ads8881
    STATIC ads8881.c
)

target_link_libraries(adc_ads8881
	PUBLIC hardware_gpio
    PUBLIC hardware_spi
    PRIVATE hal_spi
)

target_include_directories(adc_ads8881
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
