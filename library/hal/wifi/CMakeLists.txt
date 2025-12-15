message(STATUS "hal_wifi_tcp available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_wifi_tcp INTERFACE)

target_sources(hal_wifi_tcp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/wifi_tcp.c
)

target_link_libraries(hal_wifi_tcp INTERFACE
	pico_stdlib
	hardware_gpio
)
if(PICO_CYW43_SUPPORTED)
    file(COPY 
        ${CMAKE_CURRENT_SOURCE_DIR}/lwipopts.h
        DESTINATION ${CMAKE_BINARY_DIR}/..
    )
    target_link_libraries(hal_wifi_tcp INTERFACE
		pico_cyw43_arch_none
    )
endif()

target_include_directories(hal_wifi_tcp INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)


message(STATUS "hal_wifi_udp available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_wifi_udp INTERFACE)

target_sources(hal_wifi_udp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/wifi_udp.c
)

target_link_libraries(hal_wifi_udp INTERFACE
	pico_stdlib
	hardware_gpio
)
if(PICO_CYW43_SUPPORTED)
    target_link_libraries(hal_wifi_udp INTERFACE
		pico_cyw43_arch_none
    )
endif()

target_include_directories(hal_wifi_udp INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
