cmake_minimum_required(VERSION 3.9)
project(akila VERSION 0.0.1 DESCRIPTION "Development kit for graphics applications.")

add_library(akila STATIC
	"src/akila.cpp"
	"src/core/core.cpp"
	"src/ecs/coordinator.cpp"
	"src/resource/resources.cpp"
	"src/math/math.cpp"
	"src/window/window.cpp"
	"src/layer/layer.cpp"
	"src/layer/layers.cpp"
	"src/time/time.cpp"
	"src/signal/listener.cpp"
	"src/signal/signals.cpp"
	"src/signal/signal_listener.cpp"
	"src/window/window_events.cpp"
	"src/inputs/inputs.cpp"
	"src/window/imgui_handler.cpp"
	"src/threadpool/threadpool.cpp"
	"src/resource/file_system.cpp"
	"src/resource/loader.cpp"
	"src/resource/loading_instance.cpp"
	"src/renderer/renderer.cpp"
	"src/renderer/shader.cpp" 
	"src/random/random.cpp"
	"src/renderer/material.cpp"
 "src/renderer/shader_preproc.cpp" "src/default/loaders/material_loader.cpp" "src/default/layers/debug_layer.cpp" )

target_include_directories(akila PRIVATE "include")

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
