message(STATUS "hal_spi available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_spi INTERFACE)

target_sources(hal_spi INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/spi.c
)

target_link_libraries(hal_spi INTERFACE
	pico_stdlib
	hardware_gpio
	hardware_spi
)

target_include_directories(hal_spi INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/../..
)