message(STATUS "poti_ad528x available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(poti_ad528x INTERFACE)

target_sources(poti_ad528x INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad528x.c
)

target_link_libraries(poti_ad528x INTERFACE
	pico_stdlib
	hardware_gpio
    hardware_i2c
    hal_i2c
)

target_include_directories(poti_ad528x INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
