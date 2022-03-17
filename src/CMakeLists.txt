# Suggest not use file(GLOB ...)
set(SOURCES 
    component.cpp
    event.cpp
    keyboard.cpp
    nvboard.cpp
    render.cpp
    vga.cpp
    )
add_library(nvboard STATIC ${SOURCES})
install(TARGETS nvboard ARCHIVE)