add_library(hal_wudp
    STATIC wlan_udp.c
)

target_link_libraries(hal_wudp
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
)
if(PICO_CYW43_SUPPORTED)
    target_link_libraries(hal_wudp
        PUBLIC pico_btstack_classic
		PUBLIC pico_btstack_cyw43
		PUBLIC pico_cyw43_arch_none
    )
endif()

target_include_directories(hal_wudp
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
