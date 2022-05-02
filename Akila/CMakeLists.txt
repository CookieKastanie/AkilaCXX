cmake_minimum_required(VERSION 3.9)
project(akila VERSION 0.0.1 DESCRIPTION "Development kit for graphics applications.")

add_library(akila STATIC "include/akila/akila.hpp" "src/akila.cpp" "include/akila/math/math.hpp" "src/core/core.cpp" "include/akila/ecs/ecs.hpp" "include/akila/ecs/entity.hpp"  "include/akila/ecs/signature.hpp" "include/akila/ecs/componentmanager.hpp" "include/akila/ecs/coordinator.hpp" "include/akila/ecs/entitymanager.hpp" "include/akila/ecs/system.hpp" "include/akila/ecs/systemmanager.hpp" "src/ecs/coordinator.cpp" "include/akila/ecs/typename.hpp" "include/akila/ecs/entityid.hpp")

#set_target_properties(akila PROPERTIES PUBLIC_HEADER "include/akila/akila.hpp")

target_include_directories(akila PRIVATE "include")
#target_include_directories(akila PRIVATE "src")

target_compile_features(akila PRIVATE cxx_std_17)

add_subdirectory(libs/glm)
target_link_libraries(akila PUBLIC glm)
