add_library(dac_ad57x4
    STATIC ad57x4.c
)

target_link_libraries(dac_ad57x4
    PUBLIC pico_stdlib
    PUBLIC hardware_gpio
    PUBLIC hardware_spi
	PUBLIC hal_spi
)

target_include_directories(dac_ad57x4
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)