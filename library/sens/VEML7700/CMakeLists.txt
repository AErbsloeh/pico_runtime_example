message(STATUS "sens_veml7700 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_veml7700 INTERFACE)

target_sources(sens_veml7700 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/veml7700.c
)

target_link_libraries(sens_veml7700 INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_i2c
    hal_i2c
)

target_include_directories(sens_veml7700 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
