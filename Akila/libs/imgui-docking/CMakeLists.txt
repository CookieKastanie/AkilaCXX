cmake_minimum_required(VERSION 3.1.0)
project(imgui)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include")

## Files
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS "${SRC_DIR}/${PROJECT_NAME}/*.h")

add_library(${PROJECT_NAME} ${SRC} ${HEADER})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR})
