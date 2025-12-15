add_library(adc_ad4858 INTERFACE)

target_sources(adc_ad4858 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/ad4858.c
)

target_link_libraries(adc_ad4858 INTERFACE
    pico_stdlib
	hardware_gpio
    hardware_pwm
    hardware_spi
    hal_spi
)

target_include_directories(adc_ad4858 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
