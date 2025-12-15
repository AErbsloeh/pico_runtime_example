message(STATUS "hal_i2c available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_i2c INTERFACE)

target_sources(hal_i2c INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/i2c.c
)

target_link_libraries(hal_i2c INTERFACE
	pico_stdlib
	hardware_gpio
	hardware_i2c
)

target_include_directories(hal_i2c INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)