cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME TFTMusicPlayer)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
add_subdirectory(Libraries/tft)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Modules/Joystick/Joystick.c
    Modules/Serial/Serial.c
    Modules/DFPCMS/DFPCMS.c
    Modules/TFTElements/Rectangle/Rectangle.c
    Modules/TFTElements/Text/Text.c
    Modules/TFTElements/Circle/Circle.c
    Modules/TFTElements/Button/Button.c
    Modules/TFTElements/Triangle/Triangle.c
    Modules/TimeCounter/TimeCounter.c
    Modules/Screen/Screen.c
    Modules/ScreenManager/ScreenManager.c
    Core/Src/Screens/Src/screenIntro.c
    Core/Src/Screens/Src/screenBlank.c
    Core/Src/Screens/Src/screenVerification.c
    Core/Src/Screens/Src/screenMainMenu.c
    Core/Src/Screens/Src/screenPlayer.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    Modules
    Modules/Joystick
    Modules/Serial
    Modules/DFPCMS
    Modules/TFTElements/Triangle
    Modules/TFTElements/Button
    Modules/TFTElements/Circle
    Modules/TFTElements/Text
    Modules/TFTElements/Rectangle
    Modules/TimeCounter
    Modules/Screen
    Modules/ScreenManager
    Core/Src/Screens/Inc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
    tft
)
