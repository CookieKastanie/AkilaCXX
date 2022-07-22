#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <akila/akila.hpp>
#include <akila/default/loaders.hpp>
#include <akila/default/layers.hpp>
#include <akila/default/resources/static_mesh.hpp>

#include "test_layer.hpp"

using namespace akila;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return Core::run([]() {
		FileSystem::setRootFolder("resources");
		Window::setTitle("Akila !");

		Resources::registerLoader<ShaderLoader>();
		Resources::registerLoader<Texture2DLoader>();
		Resources::registerLoader<MaterialLoader>();

		ShaderPreproc::define("a_position_loc", StaticMesh::Attributes::POSITION);
		ShaderPreproc::define("a_uv_loc", StaticMesh::Attributes::UV);
		ShaderPreproc::define("a_normal_loc", StaticMesh::Attributes::NORMAL);
		ShaderPreproc::define("a_tangent_loc", StaticMesh::Attributes::TANGENT);
		ShaderPreproc::define("a_color_loc", StaticMesh::Attributes::COLOR);

		Layers::add<TestLayer>();
		Layers::add<DebugLayer>();

		Window::setSize({1920, 1080});
		Window::setPosition({100, 100});
	});
}
