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
    src/chippy_input.cpp
)
set(PROJECT_HEADERS
    src/chippy_cpu.hpp
    src/chippy_display.hpp
    src/chippy_input.hpp
    src/configuration.hpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} raylib)
