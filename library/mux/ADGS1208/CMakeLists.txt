add_library(mux_adgs1208
    STATIC adgs1208.c
)

target_link_libraries(mux_adgs1208
    PUBLIC pico_stdlib
    PUBLIC hardware_gpio
    PUBLIC hardware_spi
    PUBLIC hal_spi
)

target_include_directories(mux_adgs1208
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
