cmake_minimum_required(VERSION 3.13)
project(my_library 
	C CXX ASM
)

#include(pico_sdk_import.cmake)
pico_sdk_init()


add_subdirectory(adc)
add_subdirectory(dac)
add_subdirectory(hal)
add_subdirectory(mux)
add_subdirectory(peri)
add_subdirectory(poti)
add_subdirectory(sens)

message(STATUS "Hallo aus der Library!")