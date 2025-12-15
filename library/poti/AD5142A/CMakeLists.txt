message(STATUS "poti_ad5142a available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(poti_ad5142a INTERFACE)

target_sources(poti_ad5142a INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad5142a.c
)

target_link_libraries(poti_ad5142a INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_i2c
    hal_i2c
)

target_include_directories(poti_ad5142a INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
