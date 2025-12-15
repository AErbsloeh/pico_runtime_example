message(STATUS "hal_uart available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_uart INTERFACE)

target_sources(hal_uart INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/uart.c
)

target_link_libraries(hal_uart INTERFACE
	pico_stdlib
	hardware_gpio
	hardware_uart
)

target_include_directories(hal_uart INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/../..
)