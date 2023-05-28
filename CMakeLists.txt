cmake_minimum_required(VERSION 3.9)
project(akila VERSION 0.0.1 DESCRIPTION "Development kit for graphics applications.")

add_library(akila STATIC
	"src/core/core.cpp"
	"src/core/ecs/coordinator.cpp"
	"src/core/ecs/system_manager.cpp"
	"src/core/resources/resources.cpp"
	"src/core/math/math.cpp"
	"src/core/window/window.cpp"
	"src/core/layer/layer.cpp"
	"src/core/layer/layers.cpp"
	"src/core/time/time.cpp"
	"src/core/signals/listener.cpp"
	"src/core/signals/signals.cpp"
	"src/core/signals/signal_listener.cpp"
	"src/core/window/window_events.cpp"
	"src/core/inputs/inputs.cpp"
	"src/core/window/imgui_handler.cpp"
	"src/core/threadpool/threadpool.cpp"
	"src/core/resources/file_system.cpp"
	"src/core/resources/loader.cpp"
	"src/core/resources/loading_instance.cpp"
	"src/core/rhi/renderer.cpp"
	"src/core/rhi/shader.cpp" 
	"src/core/rhi/texture.cpp"
	"src/core/rhi/buffer_object.cpp"
	"src/core/rhi/vao.cpp"
	"src/core/audio/audio.cpp"
	"src/core/audio/audio_source.cpp"
	"src/core/audio/audio_emitter.cpp"
	"src/core/rhi/framebuffer.cpp"
	"src/core/coroutine/coroutine.cpp"
	"src/core/window/cursor.cpp"
	"src/core/inputs/joystick.cpp"
	"src/core/inputs/controller.cpp"
	"src/core/inputs/mouse.cpp"
	"src/core/inputs/keyboard.cpp"
	"src/core/inputs/gamepad.cpp"
	"src/core/inputs/input_mapping.cpp"
	
	"src/engine/engine.cpp"
	"src/engine/math_extension/easing.cpp"
	"src/engine/math_extension/distribution.cpp"
	"src/engine/math_extension/transform.cpp"
	"src/engine/containers/image.cpp"
	"src/engine/systems/render_system.cpp"
	"src/engine/components/transform_component.cpp"
	"src/engine/graphics/material.cpp"
	"src/engine/graphics/material_factory.cpp"
	"src/engine/tools/string_tools.cpp"
	"src/engine/loaders/gltf_parser.cpp"
	"src/engine/graphics/static_mesh.cpp"
	"src/engine/loaders/static_mesh_loader.cpp"
	"src/engine/loaders/bitmap_parser.cpp"
	"src/engine/loaders/texture_loader.cpp"
	"src/engine/loaders/audio_loader.cpp"
	"src/engine/loaders/material_loader.cpp"
	"src/engine/loaders/shader_source_loader.cpp"
	"src/engine/graphics/mesh.cpp"
	"src/engine/graphics/static_mesh_primitive.cpp"
	"src/engine/loaders/material_parser.cpp"
	"src/engine/graphics/camera.cpp"
	"src/engine/systems/camera_system.cpp"
	"src/engine/systems/free_camera_system.cpp"
	"src/engine/layers/debug_layer.cpp"
	"src/engine/layers/debug_timings.cpp"
	"src/engine/systems/fast3d_render_system.cpp"
	"src/engine/systems/scene_system.cpp"
	"src/engine/components/audio_component.cpp"
	"src/engine/systems/audio_system.cpp"
	"src/engine/layers/scene_layer.cpp" )

target_include_directories(akila PUBLIC "include")

target_compile_features(akila PRIVATE cxx_std_17)

add_subdirectory(lib/glm)
target_link_libraries(akila PUBLIC glm)

add_subdirectory(lib/glad)
target_link_libraries(akila PUBLIC glad)

add_subdirectory(lib/glfw)
target_link_libraries(akila PUBLIC glfw)

add_subdirectory(lib/imgui-docking)
target_link_libraries(akila PUBLIC imgui)

add_subdirectory(lib/json-develop)
target_link_libraries(akila PUBLIC nlohmann_json)

add_subdirectory(lib/stbimage)
target_link_libraries(akila PUBLIC stbimage)

add_subdirectory(lib/miniaudio)
target_link_libraries(akila PUBLIC miniaudio)

add_subdirectory(lib/minicoro)
target_link_libraries(akila PUBLIC minicoro)

add_subdirectory(lib/llvm)
target_link_libraries(akila PUBLIC llvm)

add_subdirectory(lib/cxxopts)
target_link_libraries(akila PUBLIC cxxopts)
