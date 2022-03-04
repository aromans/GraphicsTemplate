cmake_minimum_required(VERSION 3.21)
project(GraphicsTemplate)

set(CMAKE_CXX_STANDARD 23)

# ----- Include Vendors ----- #
include_directories("${PROJECT_SOURCE_DIR}/Vendor/glad/include")
include_directories("${PROJECT_SOURCE_DIR}/Vendor/stb")
include_directories("${PROJECT_SOURCE_DIR}/Vendor/glew/include")
include_directories("${PROJECT_SOURCE_DIR}/Vendor/glm/")

# ----- Include Engine ----- #
include_directories("${PROJECT_SOURCE_DIR}/Engine/src")

# ----- CMake Subdirectories ----- #
add_subdirectory("${PROJECT_SOURCE_DIR}/Vendor/GLFW")
add_subdirectory("${PROJECT_SOURCE_DIR}/Vendor/glm")
add_subdirectory("${PROJECT_SOURCE_DIR}/Vendor/spdlog")

# ----- Project CMake Subdirectories ----- #
add_subdirectory("${PROJECT_SOURCE_DIR}/Engine" "${PROJECT_SOURCE_DIR}/Engine/bin")
add_subdirectory("${PROJECT_SOURCE_DIR}/Game" "${PROJECT_SOURCE_DIR}/Game/bin")

# ----- Find Engine Library ----- #
find_library(ENGINE_LIB
        NAMES Engine
        PATHS Engine/
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH)

# ----- Linking Libraries to Projects ----- #
target_link_libraries(Engine glfw)
target_link_libraries(Game glfw)
target_link_libraries(Engine spdlog)
target_link_libraries(Game spdlog)

# ----- Linking Engine to Project ----- #
target_link_libraries(Game ${ENGINE_LIB})