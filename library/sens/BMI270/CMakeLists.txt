message(STATUS "sens_bmi270 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_bmi270_i2c
    STATIC bmi270_i2c.c
)

target_link_libraries(sens_bmi270_i2c
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_i2c
    PRIVATE hal_i2c
)

target_include_directories(sens_bmi270_i2c
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
