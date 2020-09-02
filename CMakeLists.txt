cmake_minimum_required(VERSION 3.0.0)

include(FetchContent)

set(This midi-program-changer)

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest"
    GIT_TAG        master
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set(Headers
    midi-program-changer.h
)

set(Sources
    midi-program-changer.cpp
)

add_library( 
    ${This} STATIC
    ${Sources}
    ${Headers}
)

add_subdirectory(unit_test)
