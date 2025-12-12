message(STATUS "sens_veml7700 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_veml7700
    STATIC veml7700.c
)

target_link_libraries(sens_veml7700
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_i2c
    PRIVATE hal_i2c
)

target_include_directories(sens_veml7700
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
