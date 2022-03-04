cmake_minimum_required(VERSION 3.21)
project(Game)

set(CMAKE_CXX_STANDARD 23)

# ----- Source Files ----- #
set(FILES
        src/main.cpp
        )

# ----- Build Executable ----- #
add_executable(Game ${FILES})