add_library(dac_ltc2668
    STATIC ltc2668.c
)

target_link_libraries(dac_ltc2668    
    PUBLIC pico_stdlib
    PUBLIC hardware_gpio
    PUBLIC hardware_spi
    PUBLIC hal_spi
)

target_include_directories(dac_ltc2668
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
