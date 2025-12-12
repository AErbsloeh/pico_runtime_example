add_library(hal_btspp
    STATIC bl_spp.c
)

target_link_libraries(hal_btspp
	PUBLIC pico_stdlib
	PUBLIC hardware_i2c
)
if(PICO_CYW43_SUPPORTED)
    target_link_libraries(hal_btspp
        PUBLIC pico_btstack_classic
		PUBLIC pico_btstack_cyw43
		PUBLIC pico_cyw43_arch_none
    )
endif()


target_include_directories(hal_btspp
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
