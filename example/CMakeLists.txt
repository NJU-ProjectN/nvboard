# The latest Verilator suggests cmake minimum version 3.12
cmake_minimum_required(VERSION 3.12)

project(nvboard-example)

# specify the name of the top module here
set(topName top)
# in the main.cpp, the class name of the top module is specified with macro `TOP_NAME`
add_compile_definitions(TOP_NAME=V${topName})

# find verilator
find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})

# the location of nvboard is specified by the environment variable $NVBOARD_HOME
# or you can just hardcode it here
set(nvboardDir $ENV{NVBOARD_HOME})
# build the `libnvboard.a` in subdirectory `nvboard-lib`
add_subdirectory(${nvboardDir} nvboard-lib)

# invoke the script to generate pin bindings
# specify the path to the nxdc file here
set(nxdcFile ${CMAKE_CURRENT_SOURCE_DIR}/constr/top.nxdc)
add_custom_command(
    OUTPUT auto_bind.cpp
    COMMAND ${nvboardDir}/scripts/auto_pin_bind.py ${nxdcFile} auto_bind.cpp
    DEPENDS ${nxdcFile}
)

# add all *.cpp files under `csrc` to the source list
# change the following two lines if you place your c sources elsewhere
file(GLOB srcs csrc/*.cpp)
add_executable(nvboard-example ${srcs} auto_bind.cpp)
# add the include directory of nvboard
target_include_directories(nvboard-example PRIVATE ${nvboardDir}/usr/include)
# link the excutable against the nvboard static library
target_link_libraries(nvboard-example PUBLIC nvboard)
# invoke verilator, see https://verilator.org/guide/latest/verilating.html#cmake
# change the following line if you place your verilog sources elsewhere
verilate(nvboard-example SOURCES vsrc/top.v INCLUDE_DIRS vsrc TOP_MODULE ${topName})