#include "akila/engine/engine.hpp"
#include "akila/core/resources/resources.hpp"
#include "akila/core/resources/file_system.hpp"

#include "akila/engine/graphics/material_factory.hpp"
#include "akila/engine/graphics/mesh.hpp"

#include "akila/engine/loaders/static_mesh_loader.hpp"
#include "akila/engine/loaders/texture_loader.hpp"
#include "akila/engine/loaders/audio_loader.hpp"
#include "akila/engine/loaders/material_loader.hpp"

using namespace akila;

int Engine::run(Window::InitValues const &initVals, std::function<void()> init) {
	return Core::run(initVals, [&]() {
		FileSystem::setRootFolder("resources");

		Resources::registerLoader<AudioLoader>();
		Resources::registerLoader<Texture2DLoader>();
		Resources::registerLoader<StaticMeshLoader>();
		Resources::registerLoader<MaterialLoader>();

		MaterialFactory::define("a_position_loc", Mesh::Attributes::POSITION);
		MaterialFactory::define("a_normal_loc", Mesh::Attributes::NORMAL);
		MaterialFactory::define("a_texcoord_loc", Mesh::Attributes::TEXCOORD);
		MaterialFactory::define("a_tangent_loc", Mesh::Attributes::TANGENT);
		MaterialFactory::define("a_color_loc", Mesh::Attributes::COLOR);
		MaterialFactory::define("a_joints_loc", Mesh::Attributes::JOINTS);
		MaterialFactory::define("a_weights_loc", Mesh::Attributes::WEIGHTS);
		MaterialFactory::define("a_texcoord_0_loc", Mesh::Attributes::TEXCOORD_0);
		MaterialFactory::define("a_texcoord_1_loc", Mesh::Attributes::TEXCOORD_1);
		MaterialFactory::define("a_texcoord_2_loc", Mesh::Attributes::TEXCOORD_2);
		MaterialFactory::define("a_color_0_loc", Mesh::Attributes::COLOR_0);
		MaterialFactory::define("a_color_1_loc", Mesh::Attributes::COLOR_1);
		MaterialFactory::define("a_color_2_loc", Mesh::Attributes::COLOR_2);
		MaterialFactory::define("a_joints_0_loc", Mesh::Attributes::JOINTS_0);
		MaterialFactory::define("a_joints_1_loc", Mesh::Attributes::JOINTS_1);
		MaterialFactory::define("a_joints_2_loc", Mesh::Attributes::JOINTS_2);
		MaterialFactory::define("a_weight_0_loc", Mesh::Attributes::WEIGHTS_0);
		MaterialFactory::define("a_weight_1_loc", Mesh::Attributes::WEIGHTS_1);
		MaterialFactory::define("a_weight_2_loc", Mesh::Attributes::WEIGHTS_2);
		MaterialFactory::define("a_any_loc", Mesh::Attributes::ANY);

		init();
	});
}

int Engine::run(std::function<void()> init) {
	return run({}, init);
}

void Engine::restart() {
	Core::restart();
}
