message(STATUS "mux_adg1408 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(mux_adg1408
    STATIC adg1408.c
)

target_link_libraries(mux_adg1408
    PUBLIC pico_stdlib
    PUBLIC hardware_gpio
)

target_include_directories(mux_adg1408
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
