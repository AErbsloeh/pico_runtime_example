message(STATUS "mux_adg1408 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(mux_adg1408 INTERFACE)

target_sources(mux_adg1408 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/adg1408.c
)

target_link_libraries(mux_adg1408 INTERFACE
    pico_stdlib
    hardware_gpio
)

target_include_directories(mux_adg1408 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
