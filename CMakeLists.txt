cmake_minimum_required(VERSION 3.9)
project(demo)

add_executable(demo)
target_sources(demo PRIVATE
  "Demo/src/main.cpp"
)

target_include_directories(demo PRIVATE "Demo/include")
target_compile_features(demo PRIVATE cxx_std_17)

add_subdirectory("Akila")
include_directories("Akila/include")
target_link_libraries(demo akila)

if(WIN32)
    set_target_properties(${PROJECT_NAME}
    PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
               LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()
