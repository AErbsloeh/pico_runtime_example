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

# Adding custom device/HAL library
include(FetchContent)
FetchContent_Declare(
    custom_library
    GIT_REPOSITORY https://github.com/AErbsloeh/pico_runtime_library.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(custom_library)
#add_subdirectory(${CMAKE_BINARY_DIR}/_deps/custom_library-src custom-libs)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

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
target_link_libraries(main
    pico_stdlib
    pico_bootsel_via_double_reset
    hardware_watchdog
    hardware_clocks
    hardware_gpio
    # ---- Include Hardware Abstraction Layer (HAL) libraries
    #hal_btspp
    hal_clk
    hal_i2c
    #hal_irq
    hal_led
    #hal_pwm
    #hal_pwr
    #hal_spi
    hal_tmr
    #hal_uart
    hal_usb
    #hal_wifi_tcp
    #hal_wifi_udp
     # ---- Include ADC libraries
    #adc_ad4858
    #adc_ad7779
    #adc_ads8881
    # ---- Include DAC libraries
    #dac_ad57x4
    #dac_ad5765
    #dac_ltc2668
    # ---- Include peripherie libraries
    #eth_ch9121
    #peri_sd
    #peri_w5500
    #peri_ws2812
    # ---- Include digital potentiometer libraries
    #poti_ad526x   
    #poti_ad528x
    #poti_ad5141_i2c
    #poti_ad5141_spi
    #poti_ad5142a
    # ---- Include sensor libraries
    #sens_adxl345_i2c
    #sens_bmi270_i2c
    #sens_sht21
    #sens_veml7700
    #sens_vl6180
)
# --- Extract if wireless board is available
string(FIND "${PICO_BOARD}" "_w" POSITION)
if(POSITION GREATER -1)
    add_definitions(-DPICO_CYW43_SUPPORTED)
endif()

# --- Target download if link is linked
get_target_property(LINK_LIBS main LINK_LIBRARIES)

string(FIND "${LINK_LIBS}" "peri_sd" POSITION)
if(POSITION GREATER -1)
    add_definitions(-DDOWNLOAD_SD)
endif()

string(FIND "${LINK_LIBS}" "peri_w5500" POSITION)
if(POSITION GREATER -1)
    add_definitions(-DDOWNLOAD_W5500)
endif()


# Add the standard include files to the build
target_include_directories(main PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. 
)
pico_add_extra_outputs(main)
