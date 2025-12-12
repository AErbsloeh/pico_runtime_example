message(STATUS "sens_sht21 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_sht21
    STATIC sht21.c
)

target_link_libraries(sens_sht21
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_i2c
    PRIVATE hal_i2c
)

target_include_directories(sens_sht21
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
