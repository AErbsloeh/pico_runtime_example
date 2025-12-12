# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
project(main C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Adding custom device/HAL library
add_subdirectory(../library custom-lib)

# Add executable. Default name is the project name, version 0.1
add_executable(main 
    main.c 
    callbacks/gpio_callbacks.c
    callbacks/rpc_callbacks.c
    src/daq_sample.c
    src/init_system.c
    src/testbench.c
    hardware_io.c
)

pico_set_program_name(main "main")
pico_set_program_version(main "1.0")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(main false)
pico_enable_stdio_usb(main true)

# Add the standard library to the build
add_definitions(-DPICO_BOARD="${PICO_BOARD}")
target_link_libraries(main PRIVATE
    pico_stdlib
    pico_bootsel_via_double_reset
    hardware_watchdog
    hardware_clocks
    hardware_gpio
    hal_usb
    hal_led
    hal_tmr
)

# Extract if wireless board is available
string(FIND "${PICO_BOARD}" "_w" POSITION)
if(POSITION GREATER -1)
    add_definitions(-DPICO_CYW43_SUPPORTED)
    target_link_libraries(main PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Add the standard include files to the build
target_include_directories(main PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. 
)
pico_add_extra_outputs(main)
