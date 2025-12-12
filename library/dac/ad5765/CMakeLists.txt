message(STATUS "dac_ad5765 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(dac_ad5765
    STATIC ad5765.c
)

target_link_libraries(dac_ad5765    
    PUBLIC pico_stdlib
    PUBLIC hardware_gpio
    PUBLIC hardware_spi
    PUBLIC hal_spi
)

target_include_directories(dac_ad5765
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
