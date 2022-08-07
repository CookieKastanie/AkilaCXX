#include "akila/default/loaders/audio_loader.hpp"
#include "akila/akila.hpp"

using namespace akila;

AudioLoader::AudioLoader(): Loader{"audio"} {}

void AudioLoader::onEntry(JSON json, LoaderCallback cb) {
	if(!json["name"].is_string() || !json["path"].is_string()) {
		cb.fail();
		return;
	}

	std::string name = json["name"];
	std::string path = json["path"];

	auto audio = Resources::create<AudioSource>(name);
	if(!audio->decodeFile(FileSystem::path(path))) {
		std::cerr << "Can't load audio : " << path << std::endl;
		cb.fail();
		return;
	}

	cb.success();
}
