message(STATUS "hal_uart available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_uart
    STATIC uart.c
)

target_link_libraries(hal_uart
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
	PUBLIC hardware_uart
)

target_include_directories(hal_uart
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)