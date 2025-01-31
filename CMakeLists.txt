# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(CacheSimulator VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect all source files in the src directory
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Optional: Set output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


# Optional: Enable warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
