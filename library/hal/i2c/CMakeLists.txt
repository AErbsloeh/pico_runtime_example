add_library(hal_i2c
    STATIC i2c.c
)

target_link_libraries(hal_i2c
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
	PUBLIC hardware_i2c
)


target_include_directories(hal_i2c
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)