cmake_minimum_required(VERSION 3.22.3)

project(nvboard)

set(CMAKE_INSTALL_PREFIX /usr/share/${PROJECT_NAME})
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)

include_directories(include)
add_subdirectory(src)

install(DIRECTORY include
        DESTINATION .
        FILES_MATCHING
        PATTERN *.h)

install(DIRECTORY pic
        DESTINATION .
        FILES_MATCHING
        PATTERN *.png)