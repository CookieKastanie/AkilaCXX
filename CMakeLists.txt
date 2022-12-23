cmake_minimum_required(VERSION 3.9)
project(akila VERSION 0.0.1 DESCRIPTION "Development kit for graphics applications.")

add_library(akila STATIC
	"src/core/core.cpp"
	"src/core/ecs/coordinator.cpp"
	"src/core/resource/resources.cpp"
	"src/core/math/math.cpp"
	"src/core/window/window.cpp"
	"src/core/layer/layer.cpp"
	"src/core/layer/layers.cpp"
	"src/core/time/time.cpp"
	"src/core/signal/listener.cpp"
	"src/core/signal/signals.cpp"
	"src/core/signal/signal_listener.cpp"
	"src/core/window/window_events.cpp"
	"src/core/inputs/inputs.cpp"
	"src/core/window/imgui_handler.cpp"
	"src/core/threadpool/threadpool.cpp"
	"src/core/resource/file_system.cpp"
	"src/core/resource/loader.cpp"
	"src/core/resource/loading_instance.cpp"
	"src/core/renderer/renderer.cpp"
	"src/core/renderer/shader.cpp" 
	"src/core/renderer/material.cpp"
	"src/core/renderer/shader_preproc.cpp"
	"src/core/renderer/texture.cpp"
	"src/core/renderer/buffer_object.cpp"
	"src/core/renderer/vao.cpp"
	"src/core/audio/audio.cpp"
	"src/core/audio/audio_source.cpp"
	"src/core/audio/audio_emitter.cpp"
	"src/core/renderer/framebuffer.cpp"
	"src/core/coroutine/coroutine.cpp"
	"src/core/window/cursor.cpp"
	
	"src/engine/engine.cpp"
	"src/engine/math_extension/easing.cpp"
	"src/engine/math_extension/distribution.cpp")

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

add_subdirectory(libs/miniaudio)
target_link_libraries(akila PUBLIC miniaudio)

add_subdirectory(libs/minicoro)
target_link_libraries(akila PUBLIC minicoro)
