#include "akila/resource/loader.hpp"
#include "akila/resource/loading_instance.hpp"

using namespace akila;

LoaderCallback::LoaderCallback(internal::LoadingInstance *li): li{li}, used{false} {};

void LoaderCallback::success() {
	if(used) return;
	li->countDown();
	used = true;
}

void LoaderCallback::fail() {
	if(used) return;
	li->countDown();
	used = true;
}

Loader::Loader(std::string const &listName): listName{listName} {}

std::string const &Loader::getListName() const {
	return listName;
}
