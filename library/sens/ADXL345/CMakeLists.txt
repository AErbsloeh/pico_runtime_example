add_library(sens_adxl345_i2c
    STATIC adxl345_i2c.c
)

target_link_libraries(sens_adxl345_i2c
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_i2c
    PRIVATE hal_i2c
)

target_include_directories(sens_adxl345_i2c
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
