message(STATUS "sens_sht21 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_sht21 INTERFACE)

target_sources(sens_sht21 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/sht21.c
)

target_link_libraries(sens_sht21 INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_i2c
    hal_i2c
)
 
target_include_directories(sens_sht21 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
