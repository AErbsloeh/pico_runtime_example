add_library(poti_ad5142a
    STATIC ad5142a.c
)

target_link_libraries(poti_ad5142a
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_i2c
    PRIVATE hal_i2c
)

target_include_directories(poti_ad5142a
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
