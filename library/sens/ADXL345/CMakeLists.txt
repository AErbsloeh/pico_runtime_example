message(STATUS "sens_adxl345 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_adxl345_i2c INTERFACE)

target_sources(sens_adxl345_i2c INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/adxl345_i2c.c
)

target_link_libraries(sens_adxl345_i2c INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_i2c
    hal_i2c
)

target_include_directories(sens_adxl345_i2c INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
