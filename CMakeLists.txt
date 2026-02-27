cmake_minimum_required(VERSION 3.27.7)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)

set(NAME fluidSim)

project(${NAME})

find_package(raylib 4.5 REQUIRED)
find_package(OpenMP)

file(GLOB_RECURSE SOURCE_FILES "code/*.cpp" "code/*.h")

add_executable(${NAME} ${SOURCE_FILES})

# Reasonable defaults for performance-oriented simulation even when building Debug.
# (You still get symbols via -g from the Debug configuration.)
target_compile_options(${NAME} PRIVATE
    $<$<CONFIG:Debug>:-O1>
    $<$<CONFIG:Release>:-O3>
)

target_link_libraries(${NAME} raylib)
if (OpenMP_CXX_FOUND)
    target_link_libraries(${NAME} OpenMP::OpenMP_CXX)
endif()
