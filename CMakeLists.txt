cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(chippy)

add_subdirectory(SDL)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} SDL2main SDL2-static)