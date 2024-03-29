cmake_minimum_required(VERSION 3.0.0)
project(stevesStation)

set(APP_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(EXTERNALS_DIR "${PROJECT_SOURCE_DIR}/extern")
set(RAYLIB_DIR "${EXTERNALS_DIR}/raylib/")

set(CMAKE_CXX_STANDARD 17)

add_executable(
    stevesStation
    ${APP_SOURCE_DIR}/pch.h
    ${APP_SOURCE_DIR}/main.cpp
    ${APP_SOURCE_DIR}/global.h
    ${APP_SOURCE_DIR}/global_no_headers.h
)

target_include_directories(
    stevesStation
    PRIVATE ${EXTERNALS_DIR}
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
add_subdirectory("${RAYLIB_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
