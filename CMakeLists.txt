cmake_minimum_required(VERSION 3.29)

project(nvboard LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Python3 REQUIRED Interpreter)

message(STATUS "Using Python interpreter: ${Python3_EXECUTABLE}")
set(Python3_EXECUTABLE ${Python3_EXECUTABLE} CACHE INTERNAL "Path to Python executable")
set(BIND_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/scripts/auto_pin_bind.py CACHE INTERNAL "Path to Python scripts")
set(ENV{NVBOARD_HOME} ${CMAKE_CURRENT_SOURCE_DIR})

function(auto_bind target constraint_file output_file)
    message("Generating bind file")
    add_custom_command(
        OUTPUT ${output_file}
        COMMAND ${CMAKE_COMMAND} -E env NVBOARD_HOME=$ENV{NVBOARD_HOME} ${Python3_EXECUTABLE} ${BIND_SCRIPT} ${constraint_file} ${output_file}
        DEPENDS ${constraint_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction(auto_bind)

add_library(nvboard
    SHARED
    src/button.cpp 
    src/component.cpp 
    src/event.cpp 
    src/font.cpp 
    src/keyboard.cpp 
    src/led.cpp 
    src/nvboard.cpp 
    src/render.cpp 
    src/segs7.cpp 
    src/switch.cpp 
    src/term.cpp 
    src/timer.cpp
    src/uart.cpp 
    src/vga.cpp
)
target_link_libraries(nvboard PRIVATE SDL2::SDL2 SDL2_image SDL2_ttf)
target_include_directories(nvboard PRIVATE include)
target_include_directories(nvboard INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/usr/include)
target_compile_options(nvboard PRIVATE -O3)