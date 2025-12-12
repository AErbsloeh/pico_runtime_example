add_library(poti_ad528x
    STATIC ad528x.c
)

target_link_libraries(poti_ad528x
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
    PUBLIC hardware_i2c
    PRIVATE hal_i2c
)

target_include_directories(poti_ad528x
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
