message(STATUS "sens_vl6180 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_vl6180 INTERFACE)

target_sources(sens_vl6180 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/vl6180.c
)

target_link_libraries(sens_vl6180 INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_i2c
    hal_i2c
)

target_include_directories(sens_vl6180 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
