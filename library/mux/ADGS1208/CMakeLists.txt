message(STATUS "mux_adgs1208 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(mux_adgs1208 INTERFACE)

target_sources(mux_adgs1208 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/adgs1208.c
)

target_link_libraries(mux_adgs1208 INTERFACE
    pico_stdlib
    hardware_gpio
    hardware_spi
    hal_spi
)

target_include_directories(mux_adgs1208 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
