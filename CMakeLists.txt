cmake_minimum_required(VERSION 3.10)

project(OpenGLApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/camera.cpp
    src/lodepng.cpp
    src/main.cpp
    src/model.cpp
    src/shader.cpp
    src/window.cpp
    src/collision.cpp
)

add_executable(app ${SOURCES})

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(app PRIVATE -Wall)

target_link_libraries(app PRIVATE GL glfw GLEW assimp)