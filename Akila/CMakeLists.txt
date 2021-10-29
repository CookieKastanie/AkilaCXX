project("Akila")

set(CMAKE_CXX_STANDARD 17)

set(RESOURCES_DIRECTORY "${PROJECT_SOURCE_DIR}/resources")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include")

option(USE_IMGUI "Enable imgui" OFF)

if(USE_IMGUI)
    add_compile_definitions(IMGUI)
endif(USE_IMGUI)

## Lib dependency
find_package(OpenGL REQUIRED)
add_subdirectory(libs)

## Files
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS "${SRC_DIR}/${PROJECT_NAME}/*.hpp")

set(RESOURCE_FILES)

add_library(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR})

if(USE_IMGUI)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} stbimage glad glfw glm imgui)
else(USE_IMGUI)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} stbimage glad glfw glm)
endif(USE_IMGUI)
