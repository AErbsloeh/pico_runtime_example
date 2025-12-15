message(STATUS "hal_tmr available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_tmr INTERFACE)

target_sources(hal_tmr INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/tmr.c
)

target_link_libraries(hal_tmr INTERFACE
	pico_stdlib
	hardware_clocks
)

target_include_directories(hal_tmr INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)