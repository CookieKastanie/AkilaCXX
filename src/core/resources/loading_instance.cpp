#include "akila/core/resources/loading_instance.hpp"
#include "akila/core/resources/file_system.hpp"
#include "akila/core/resources/json.hpp"
#include "akila/core/resources/resources.hpp"
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
			JSON jsonFile = JSON::parse(std::ifstream{FileSystem::resources(path)}, nullptr, true, true);

			if(!jsonFile.is_object()) return;

			for(auto &item : jsonFile.items()) {
				auto &value = item.value();
				if(value.is_array() == false) continue;

				auto &arr = completJSON[item.key()];
				if(arr.is_array() == false) arr = JSON::array();
				count += value.size();
				arr.insert(arr.end(), value.begin(), value.end());
			}
		} catch(std::exception const&) {
			std::cerr << "JSON file error : can't read " << FileSystem::resources(path) << std::endl;
		}
	}

	
	LoaderCallback loaderCB{this};
	
	// verif existance des loaders
	for(auto &json : completJSON.items()) {
		bool found = false;
		for(auto &loader : Resources::loaders) {
			if(loader->getListName() == json.key()) {
				found = true;
				continue;
			}
		}

		if(found == false) {
			std::cerr << "Missing loader for " << json.key() << std::endl;
			count -= json.value().size() - 1;
			countDown();
		}
	}
	
	// appelle des loaders dans l'ordre
	for(auto &loader : Resources::loaders) {
		std::string const &listName = loader->getListName();
		if(completJSON[listName].is_array() == false) continue;

		for(JSON &value : completJSON[listName]) {
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
