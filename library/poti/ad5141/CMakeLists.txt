message(STATUS "poti_ad5141_i2 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(poti_ad5141_i2c INTERFACE)

target_sources(poti_ad5141_i2c INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad5141_i2c.c
)

target_link_libraries(poti_ad5141_i2c INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_i2c
    hal_i2c
)

target_include_directories(poti_ad5141_i2c INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)


message(STATUS "poti_ad5141_spi available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(poti_ad5141_spi INTERFACE)

target_sources(poti_ad5141_spi INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad5141_spi.c
)

target_link_libraries(poti_ad5141_spi INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_spi
    hal_spi
)

target_include_directories(poti_ad5141_spi INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
