add_library(peri_w5500
    usb.c
)

target_link_libraries(peri_w5500
	PUBLIC pico_stdlib
	PUBLIC hardware_gpio
)

target_include_directories(peri_w5500
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# --- Fetch external libs from GitHub ---
include(FetchContent)
FetchContent_Declare(
    wiznet_pico_c
    GIT_REPOSITORY https://github.com/WIZnet-ioNIC/WIZnet-PICO-C.git
    GIT_TAG main  
)
#FetchContent_MakeAvailable(wiznet_pico_c)