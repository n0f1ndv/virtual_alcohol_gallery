cmake_minimum_required(VERSION 3.10)

project(OpenGLApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather source files (Equivalent to SOURCE = $(wildcard src/*))
# NOTE: While GLOB mimics Makefile's wildcard, CMake best practice 
# is to list source files explicitly so it knows when a new file is added.
file(GLOB SOURCES "src/*.cpp") 

add_executable(app ${SOURCES})

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(app PRIVATE -Wall)

target_link_libraries(app PRIVATE GL glfw GLEW assimp)
