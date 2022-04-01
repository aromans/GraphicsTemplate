cmake_minimum_required(VERSION 3.21)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

# ----- Source Files ----- #
set(FILES
        src/Engine/Core/Application.cpp
        src/Engine/Core/Platform/GLWindow.cpp
        src/Engine/Core/Platform/GLInput.cpp
        src/Engine/Core/Logger/Log.cpp
        )

# ----- Build Static Library ----- #
add_library(Engine STATIC ${FILES})

