# --- Fetch external libs from GitHub ---
include(FetchContent)
FetchContent_Declare(
    pico_sd
    GIT_REPOSITORY https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico.git
    GIT_TAG        main
)
if(DOWNLOAD_SD)
    FetchContent_MakeAvailable(pico_sd)
    add_subdirectory(${CMAKE_BINARY_DIR}/_deps/pico_sd-src/src build)
endif()

# --- Link into project
message(STATUS "peri_sd available at: ${CMAKE_BINARY_DIR}/_deps/pico_sd-src/src")
add_library(peri_sd INTERFACE)

target_sources(peri_sd INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/sd_spi.c
)

target_link_libraries(peri_sd INTERFACE
	pico_stdlib
	hardware_gpio
	hardware_spi
	hal_spi
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
)

target_include_directories(peri_sd INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
