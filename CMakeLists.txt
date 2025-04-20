# The latest Verilator suggests cmake minimum version 3.12
cmake_minimum_required(VERSION 3.12)

project(nvboard)

# find SDL2, see https://wiki.libsdl.org/SDL2/README/cmake
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
find_package(SDL2_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL2_ttf)

# add all *.cpp files under src to the source list
file(GLOB nvboard_srcs src/*.cpp)
# add the `libnvboard.a` static library
add_library(nvboard STATIC ${nvboard_srcs})
target_include_directories(nvboard PRIVATE include)
target_link_libraries(nvboard PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)