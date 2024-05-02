cmake_minimum_required(VERSION 3.27.7)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)

set(NAME fluidSim)

project(${NAME})

find_package(raylib 4.5 REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "code/*.cpp" "code/*.h")

add_executable(${NAME} ${SOURCE_FILES})

target_link_libraries(${NAME} raylib)