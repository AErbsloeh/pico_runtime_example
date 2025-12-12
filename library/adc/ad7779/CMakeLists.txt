message(STATUS "adc_ad7779 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(adc_ad7779
    STATIC ad7779.c
)

target_link_libraries(adc_ad7779
    PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_spi
    PRIVATE hal_spi
)

target_include_directories(adc_ad7779
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
