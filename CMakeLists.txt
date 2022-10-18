cmake_minimum_required(VERSION 3.15)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
project(theecpp CXX C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")


# system detection "stolen" from sokol cmake build
if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(EMSCRIPTEN 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL iOS)
    set(OSX_IOS 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL Android)
    set(ANDROID 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(LINUX 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(OSX_MACOS 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL WindowsStore)
    set(UWP 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(WINDOWS 1)
else()
    message(FATAL_ERROR "Unrecognized CMAKE_SYSTEM_NAME")
endif()


add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

add_library(three::project_options ALIAS project_options)
add_library(three::project_warnings ALIAS project_warnings)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(examples)
