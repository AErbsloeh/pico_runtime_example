message(STATUS "poti_ad526x available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(poti_ad526x INTERFACE)

target_sources(poti_ad526x INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad526x.c
)

target_link_libraries(poti_ad526x INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_spi
    hal_spi
)

target_include_directories(poti_ad526x INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
