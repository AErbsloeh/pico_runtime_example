message(STATUS "sens_bmi270 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_bmi270_i2c INTERFACE)

target_sources(sens_bmi270_i2c INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/bmi270_i2c.c
)

target_link_libraries(sens_bmi270_i2c INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_i2c
    hal_i2c
)

target_include_directories(sens_bmi270_i2c INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
