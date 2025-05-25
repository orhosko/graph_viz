cmake_minimum_required(VERSION 3.13)
project(graph_viz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure we're using Emscripten
if(NOT DEFINED EMSCRIPTEN)
    message(FATAL_ERROR "This project must be compiled with Emscripten")
endif()

add_executable(graph_viz add.cpp)

# Set Emscripten specific options
set_target_properties(graph_viz PROPERTIES
    SUFFIX ".js"
    LINK_FLAGS "-s WASM=1 -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=['run'] -lembind --bind"
) 