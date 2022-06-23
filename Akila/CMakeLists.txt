cmake_minimum_required(VERSION 3.9)
project(akila VERSION 0.0.1 DESCRIPTION "Development kit for graphics applications.")

add_library(akila STATIC "include/akila/akila.hpp" "src/akila.cpp" "include/akila/math/math.hpp" "src/core/core.cpp" "include/akila/ecs/ecs.hpp" "include/akila/ecs/entity.hpp"  "include/akila/ecs/signature.hpp" "include/akila/ecs/component_manager.hpp" "include/akila/ecs/coordinator.hpp" "include/akila/ecs/entity_manager.hpp" "include/akila/ecs/system.hpp" "include/akila/ecs/system_manager.hpp" "src/ecs/coordinator.cpp" "include/akila/common/type_infos.hpp" "include/akila/ecs/entity_id.hpp" "include/akila/core/Core.hpp" "include/akila/memory/ptr.hpp" "include/akila/memory/ref.hpp" "include/akila/resource/resources.hpp" "include/akila/resource/resource_map.hpp" "src/resource/resources.cpp" "src/math/math.cpp" "include/akila/window/window.hpp" "src/window/window.cpp" "include/akila/inputs/inputs.hpp" "include/akila/layer/layer.hpp" "include/akila/layer/layers.hpp" "src/layer/layer.cpp" "src/layer/layers.cpp" "include/akila/time/Time.hpp" "src/time/time.cpp" "include/akila/signal/signals.hpp" "include/akila/signal/listener.hpp" "src/signal/listener.cpp" "src/signal/signals.cpp" "include/akila/signal/signal_queue.hpp" "include/akila/signal/signal_dispatcher.hpp" "include/akila/signal/signal_listener.hpp" "include/akila/signal/signal_callback.hpp" "src/signal/signal_listener.cpp" "include/akila/window/window_events.hpp" "src/window/window_events.cpp" "src/inputs/inputs.cpp" "include/akila/window/imgui_handler.hpp" "src/window/imgui_handler.cpp" "include/akila/threadpool/threadpool.hpp"  "src/threadpool/threadpool.cpp" "include/akila/resource/file_system.hpp" "src/resource/file_system.cpp" "include/akila/resource/loader.hpp" "include/akila/resource/loading_instance.hpp" "include/akila/resource/json.hpp" "src/resource/loader.cpp" "src/resource/loading_instance.cpp" "include/akila/renderer/renderer.hpp" "src/renderer/renderer.cpp" "include/akila/renderer/shader.hpp" "src/renderer/shader.cpp" "include/akila/random/random.hpp" "src/random/random.cpp" "src/renderer/material.cpp")

#set_target_properties(akila PROPERTIES PUBLIC_HEADER "include/akila/akila.hpp")

target_include_directories(akila PRIVATE "include")
#target_include_directories(akila PRIVATE "src")

target_compile_features(akila PRIVATE cxx_std_17)

add_subdirectory(libs/glm)
target_link_libraries(akila PUBLIC glm)

add_subdirectory(libs/glad)
target_link_libraries(akila PUBLIC glad)

add_subdirectory(libs/glfw)
target_link_libraries(akila PUBLIC glfw)

add_subdirectory(libs/imgui-docking)
target_link_libraries(akila PUBLIC imgui)

add_subdirectory(libs/json-develop)
target_link_libraries(akila PUBLIC nlohmann_json)

add_subdirectory(libs/stbimage)
target_link_libraries(akila PUBLIC stbimage)
