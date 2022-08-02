#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <akila/akila.hpp>
#include <akila/default/loaders.hpp>
#include <akila/default/layers.hpp>
#include <akila/default/resources/static_mesh.hpp>

#include "utils_layer.hpp"

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

		Layers::add<UtilsLayer>(Layer::Depth::BACK);
		Layers::add<DebugLayer>();

		Window::setSize({1600, 900});
		Window::setPositionToCenter();


		Ref<AudioSource> audioBuffer = Resources::create<AudioSource>("hound");
		audioBuffer->decodeFile(FileSystem::path("audio/the_hound.mp3"));

		Ref<AudioSource> audioBuffer2 = Resources::create<AudioSource>("boom");
		audioBuffer2->decodeFile(FileSystem::path("audio/boom.mp3"));

		//Resources::create<AudioEmitter>("hound", *audioBuffer);
		//Resources::create<AudioEmitter>("boom", *audioBuffer2);
	});
}
