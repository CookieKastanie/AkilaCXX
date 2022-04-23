cmake_minimum_required(VERSION 3.1.0)
project("Akila")

set(CMAKE_CXX_STANDARD 17)

set(RESOURCES_DIRECTORY "${PROJECT_SOURCE_DIR}/resources")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include")

option(USE_IMGUI "Enable imgui" OFF)

if(USE_IMGUI)
    add_compile_definitions(IMGUI)
endif(USE_IMGUI)

## Files
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS "${SRC_DIR}/${PROJECT_NAME}/*.hpp")

set(RESOURCE_FILES)

add_library(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR})

####################################################

## Libs dependency
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARY})

add_subdirectory(libs/stbimage)
target_link_libraries(${PROJECT_NAME} PUBLIC stbimage)

add_subdirectory(libs/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(libs/glfw-3.3.2)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(libs/glm-0.9.9.9/glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

if(USE_IMGUI)
    add_subdirectory(libs/imgui-docking)
    target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
endif(USE_IMGUI)

add_subdirectory(libs/json-develop)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json)
