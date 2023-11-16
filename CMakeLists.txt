cmake_minimum_required(VERSION 3.27)
project(chippy)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

# Our project files
set(PROJECT_SOURCES
    src/main.cpp
)

set(PROJECT_INCLUDES

)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_INCLUDES} ${raylib_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${raylib_LIBRARIES})