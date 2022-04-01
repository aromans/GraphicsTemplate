cmake_minimum_required(VERSION 3.21)
project(GraphicsTemplate)

set(CMAKE_CXX_STANDARD 23)

# ----- Find Engine Library ----- #
find_library(ENGINE_LIB
        NAMES Engine
        PATHS Engine/
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH)

# ----- Building Engine Files ----- #
if (NOT EXISTS ${ENGINE_LIB})
    EXECUTE_PROCESS(COMMAND sh BuildEngine.sh
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Engine/
                    RESULT_VARIABLE ENGINE_BUILT)
    if (NOT ENGINE_BUILT EQUAL "0")
        message(Could not find the Engine static lib file!)
    else()
        find_library(ENGINE_LIB
                NAMES Engine
                PATHS Engine/
                PATH_SUFFIXES bin
                NO_DEFAULT_PATH)
    endif()
endif()

# ----- Include Vendors ----- #
include_directories("${PROJECT_SOURCE_DIR}/Vendor/glad/include")
include_directories("${PROJECT_SOURCE_DIR}/Vendor/stb")
include_directories("${PROJECT_SOURCE_DIR}/Vendor/glew/include")
#include_directories("${PROJECT_SOURCE_DIR}/Vendor/GLFW/include")
include_directories("${PROJECT_SOURCE_DIR}/Vendor/glm/")


# ----- Include Engine ----- #
include_directories("${PROJECT_SOURCE_DIR}/Engine/src")

# ----- CMake Subdirectories ----- #
add_subdirectory("${PROJECT_SOURCE_DIR}/Vendor/GLFW")
add_subdirectory("${PROJECT_SOURCE_DIR}/Vendor/glm")
add_subdirectory("${PROJECT_SOURCE_DIR}/Vendor/spdlog")
add_subdirectory("${PROJECT_SOURCE_DIR}/Vendor/glad")
add_subdirectory("${PROJECT_SOURCE_DIR}/Vendor/glew")

# ----- Project CMake Subdirectories ----- #
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine" "${PROJECT_SOURCE_DIR}/Engine/bin")
add_subdirectory("${PROJECT_SOURCE_DIR}/Game" "${PROJECT_SOURCE_DIR}/Game/bin")

# ----- Linking Libraries to Projects ----- #
target_link_libraries(Engine glfw)
target_link_libraries(Game glfw)
target_link_libraries(Engine spdlog)
target_link_libraries(Game spdlog)
target_link_libraries(Engine glad)
target_link_libraries(Game glad)
target_link_libraries(Engine glew)
target_link_libraries(Game glew)

# ----- Linking Engine to Project ----- #
target_link_libraries(Game ${ENGINE_LIB})

if (APPLE)
    target_link_libraries(Engine "-framework OpenGL")
    target_link_libraries(Game "-framework OpenGL")
endif()
