cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")

file(GLOB_RECURSE EZ_SOURCES CONFIGURE_DEPENDS "src/ez/*.cpp")

add_library(ez ${EZ_SOURCES})

target_compile_definitions(ez PUBLIC GLFW_INCLUDE_NONE)
#target_compile_definitions(ez PUBLIC EZ_BUILD_ENABLE_PROFILING)
target_compile_definitions(ez PUBLIC EZ_BUILD_ENABLE_ASSERTIONS)
target_compile_definitions(ez PUBLIC EZ_BUILD_DEBUG_MODE)


target_include_directories(ez SYSTEM PUBLIC "vendor/glm" "vendor/glad" "vendor/stb" "vendor/glfw/include" )
target_include_directories(ez PUBLIC "src")

add_subdirectory(vendor/glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(ez OpenGL::GL glfw)