cmake_minimum_required(VERSION 3.21)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

# ----- Build Static Library ----- #
set(FILES
        src/library.cpp
        )

# ----- Build Static Library ----- #
add_library(Engine STATIC ${FILES})

