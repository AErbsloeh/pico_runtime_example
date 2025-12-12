add_library(poti_ad5141_spi
    STATIC ad5141_spi.c
)

target_link_libraries(poti_ad5141_spi
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_spi
    PRIVATE hal_spi
)

target_include_directories(poti_ad5141_spi
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
