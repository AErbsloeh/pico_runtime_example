message(STATUS "hal_irq available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(hal_irq INTERFACE)

target_sources(hal_irq INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/irq.c
)

target_link_libraries(hal_irq INTERFACE
	pico_stdlib
	hardware_gpio
)

target_include_directories(hal_irq INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
