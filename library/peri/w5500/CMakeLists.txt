# --- Fetch external libs from GitHub ---
include(FetchContent)
FetchContent_Declare(
    wiznet_pico_c
    GIT_REPOSITORY https://github.com/WIZnet-ioNIC/WIZnet-PICO-C.git
    GIT_TAG main  
)
if(DOWNLOAD_W5500)
    FetchContent_MakeAvailable(wiznet_pico_c)
    add_subdirectory(${CMAKE_BINARY_DIR}/_deps/wiznet_pico_c-src/src build)
endif()

# --- Link to project ---
message(STATUS "peri_w5500 available at: ${CMAKE_CURRENT_LIST_FILE}")
add_library(peri_w5500 INTERFACE)

target_sources(peri_w5500 INTERFACE)

target_link_libraries(peri_w5500 INTERFACE
	pico_stdlib
	hardware_gpio
)

target_include_directories(peri_w5500 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
