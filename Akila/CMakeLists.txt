project("Akila")

set(RESOURCES_DIRECTORY "${PROJECT_SOURCE_DIR}/resources")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include")

## Lib dependency
find_package(OpenGL REQUIRED)
add_subdirectory(libs)

## Files
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS "${SRC_DIR}/${PROJECT_NAME}/*.hpp")

set(RESOURCE_FILES)

add_library(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
target_include_directories (${PROJECT_NAME} PUBLIC ${INC_DIR})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} stbimage glad glfw glm 
#assimp 
imgui)
