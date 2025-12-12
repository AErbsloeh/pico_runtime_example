message(STATUS "sens_vl6180 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(sens_vl6180
    STATIC vl6180.c
)

target_link_libraries(sens_vl6180
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_i2c
    PRIVATE hal_i2c
)

target_include_directories(sens_vl6180
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
