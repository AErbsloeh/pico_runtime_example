message(STATUS "hal_pwr available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_pwr
    STATIC pwr_watch.c
)

target_link_libraries(hal_pwr
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
)

target_include_directories(hal_pwr
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)