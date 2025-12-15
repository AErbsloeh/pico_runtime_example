message(STATUS "hal_pwr available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_pwr INTERFACE)

target_sources(hal_pwr INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/pwr_watch.c
)

target_link_libraries(hal_pwr INTERFACE
	pico_stdlib
	hardware_gpio
)

target_include_directories(hal_pwr INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
