cmake_minimum_required(VERSION 3.10.2)

project(ImGuiAndroidSoftKeyboard)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/Includes
)

# Source files
set(SOURCES
    main.cpp
    SoftKeyboard.cpp
    menu.cpp
)

# Create shared library
add_library(BossUnknownX SHARED ${SOURCES})

# Link libraries
target_link_libraries(BossUnknownX
    android
    EGL
    GLESv3
    log
)

# Compiler flags
target_compile_options(BossUnknownX PRIVATE
    -Wall
    -Wextra
    -fPIC
    -O2
)

# Set output directory
set_target_properties(BossUnknownX PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}
)