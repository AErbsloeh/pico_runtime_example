add_library(poti_ad526x
    STATIC ad526x.c
)

target_link_libraries(poti_ad526x
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_spi
    PRIVATE hal_spi
)

target_include_directories(poti_ad526x
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
