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
file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS "${RESOURCES_DIRECTORY}/*")

set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/../Akila/resources/Akila.rc)

add_executable(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES} ${APP_ICON})

if(WIN32)
    set_target_properties(${PROJECT_NAME}
    PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
               LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

include_directories(${INC_DIR} AFTER)

target_link_libraries(${PROJECT_NAME} Akila)



#Copy resources
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
