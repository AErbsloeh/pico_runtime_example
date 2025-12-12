add_library(peri_sd
    usb.c
)

target_link_libraries(peri_sd
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
	PUBLIC hardware_spi
)

target_include_directories(peri_sd
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)