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
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(main C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(main main.c 
    hal/i2c.c
    hal/led.c
    hal/spi.c
    hal/tmr.c  
    hal/usb.c
    src/gpio_callbacks.c
    src/init_system.c
    src/testbench.c
    src/usb_callbacks.c
    hardware_io.c
)

pico_set_program_name(main "main")
pico_set_program_version(main "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(main 0)
pico_enable_stdio_usb(main 1)

# Add the standard library to the build
target_link_libraries(main
    pico_stdlib
    pico_bootsel_via_double_reset
    hardware_clocks
    hardware_pio
    hardware_spi
    hardware_i2c
)
add_definitions(-DPICO_BOARD="${PICO_BOARD}")

# Extract if wireless board is available
string(FIND "${PICO_BOARD}" "_w" POSITION)
if(POSITION GREATER -1)
    target_link_libraries(main
        pico_btstack_classic
        pico_btstack_cyw43
        pico_cyw43_arch_none
    )
    add_definitions(-DPICO_CYW43_SUPPORTED)
endif()


# Add the standard include files to the build
target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(main)

# ==================== TESTING =========================
macro(add_ctest)
    include(CTest)
endmacro()

macro(add_unity)
    FetchContent_Declare(
            unity
            GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
            GIT_TAG v2.5.2
            OVERRIDE_FIND_PACKAGE)
    FetchContent_MakeAvailable(unity)
    find_package(unity)
endmacro()

function(__register_target_as_unit_test target)
    if(NOT TARGET all_unit_tests)
        add_custom_target(all_unit_tests)
    endif ()
    add_dependencies(all_unit_tests ${target})

    add_test(${target} ${target})
    set_property(TEST ${target} PROPERTY LABELS unit)
endfunction()

function(add_elastic_ai_unit_test)
    # You only need to specify the LIB_UNDER_TEST.
    # The function will automatically try to compile a file with the same name, but
    # the prefix `Unittest` and link against LIB_UNDER_TEST.
    # For flexibility you can specify additional libraries (MORE_LIBS) and
    # sources (MORE_SOURCES).
    set(oneValueArgs LIB_UNDER_TEST)
    set(multiValueArgs MORE_SOURCES MORE_LIBS)
    cmake_parse_arguments(PARSE_ARGV 0 arg
            "${options}" "${oneValueArgs}" "${multiValueArgs}"
    )
    string(CONCAT NAME "UnitTest" ${arg_LIB_UNDER_TEST})
    if(NOT arg_MORE_SOURCES)
        set(arg__MORE_SOURCES "")
    endif ()

    if(NOT arg_MORE_LIBS)
        set(arg_MORE_LIBS "")
    endif ()

    if (NOT arg_DEPS)
        set(arg_DEPS "")
    endif ()


    add_executable(${NAME} ${NAME}.c)
    target_link_libraries(${NAME} PRIVATE
            ${arg_LIB_UNDER_TEST}
            unity::framework
            ${arg_MORE_LIBS})
    __register_target_as_unit_test(${NAME})

endfunction()
