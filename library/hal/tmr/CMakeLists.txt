add_library(hal_tmr
    STATIC tmr.c
)

target_link_libraries(hal_tmr
	PUBLIC pico_stdlib
	PUBLIC hardware_clocks
)

target_include_directories(hal_tmr
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)