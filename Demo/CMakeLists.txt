cmake_minimum_required(VERSION 3.0.2)
project("Demo")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD "17")

set(RESOURCES_DIRECTORY "${PROJECT_SOURCE_DIR}/resources")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include")

## Files
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS "${SRC_DIR}/${PROJECT_NAME}/*.hpp")

set(RESOURCE_FILES)

add_executable(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
include_directories(${INC_DIR} AFTER)

target_link_libraries(${PROJECT_NAME} Akila)
