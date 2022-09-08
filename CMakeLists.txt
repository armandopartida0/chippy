cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(chippy)

# raylib
find_package(SDL2 QUIET)
if (NOT SDL2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        sdl2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-2.24.0
    )
    FetchContent_MakeAvailable(sdl2)
endif()

include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})