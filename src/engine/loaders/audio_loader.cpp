#include "akila/engine/loaders/audio_loader.hpp"
#include "akila/core/audio/audio_source.hpp"
#include "akila/core/resources/resources.hpp"
#include "akila/core/resources/file_system.hpp"

using namespace akila;

AudioLoader::AudioLoader(): Loader{"audio"} {}

void AudioLoader::onEntry(JSON json, LoaderCallback cb) {
	if(
		json["name"].is_string() == false || 
		json["path"].is_string() == false
	) {
		cb.fail();
		return;
	}

	std::string name = json["name"];
	std::string path = json["path"];
	
	// TODO
	//bool streamMode = false;
	//if(json["stream"].is_boolean()) {
	//	streamMode = json["stream"];
	//}
	

	auto audio = Resources::create<AudioSource>(name);
	if(audio->decodeFile(FileSystem::resources(path)) == false) {
		std::cerr << "Can't load audio : " << path << std::endl;
		cb.fail();
		return;
	}

	cb.success();
}
