add_library(hal_spi
    STATIC spi.c
)

target_link_libraries(hal_spi
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
	PUBLIC hardware_spi
)

target_include_directories(hal_spi
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)