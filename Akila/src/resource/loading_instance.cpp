#include "akila/resource/loading_instance.hpp"
#include "akila/resource/file_system.hpp"
#include "akila/resource/json.hpp"
#include "akila/resource/resources.hpp"
#include <iostream>

using namespace akila::internal;

LoadingInstance::LoadingInstance(std::string const &path, std::function<void()> const &callback):
	finishedCallback{callback}, paths{{path}}, count{0} {}

LoadingInstance::LoadingInstance(std::initializer_list<std::string> const &paths, std::function<void()> const &callback) :
	finishedCallback{callback}, paths{paths}, count{0} {}

void LoadingInstance::start() {
	JSON completJSON;

	// concatenation et verification des json
	for(std::string const &path : paths) {
		try {
			JSON jsonFile;
			std::ifstream{FileSystem::path(path)} >> jsonFile;
			
			if(!jsonFile.is_object()) return;

			for(auto &item : jsonFile.items()) {
				auto &value = item.value();
				if(!value.is_array()) continue;

				auto &arr = completJSON[item.key()];
				if(!arr.is_array()) arr = JSON::array();
				count += value.size();
				arr.insert(arr.end(), value.begin(), value.end());
			}
		} catch(std::exception const&) {
			std::cerr << "JSON file error : can't read " << path << std::endl;
		}
	}

	// appelle des differents loaders
	LoaderCallback loaderCB{this};
	for(auto &json : completJSON.items()) {
		auto loaderIt = Resources::loaders.find(json.key());
		if(loaderIt == Resources::loaders.end()) {
			std::cerr << "Missing loader for " << json.key() << std::endl;
			count -= json.value().size() - 1;
			countDown();
			continue;
		}

		Loader *loader = loaderIt->second.get();
		for(JSON &value : json.value()) {
			loader->onEntry(value, loaderCB);
		}
	}
}

void LoadingInstance::countDown() {
	if(--count == 0) {
		finishedCallback();

		// se retire de la liste tout seul
		for(std::size_t i = 0; i < Resources::loadingInstances.size(); ++i) {
			if(&Resources::loadingInstances[i] == this) {
				Resources::loadingInstances.erase(Resources::loadingInstances.begin() + i);
			}
		}
	}
}
