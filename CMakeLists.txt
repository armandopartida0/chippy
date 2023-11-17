cmake_minimum_required(VERSION 3.27)
project(chippy)

# Adding raylib (Might use raylib-cpp in future, for now just assume we have
# raylib built somewhere)
find_package(raylib QUIET)

# Our project files
set(PROJECT_SOURCES
    src/main.cpp
    src/chippy_cpu.cpp
    src/chippy_display.cpp
)

set(PROJECT_INCLUDES
    src/chippy_cpu.hpp
    src/chippy_display.hpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(chippy PRIVATE include)
target_link_libraries(${PROJECT_NAME} raylib)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
