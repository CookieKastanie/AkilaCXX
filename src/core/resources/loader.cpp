#include "akila/core/resources/loader.hpp"
#include "akila/core/resources/loading_instance.hpp"

using namespace akila;

LoaderCallback::LoaderCallback(internal::LoadingInstance *li): li{li}, used{false} {};

void LoaderCallback::success() const {
	if(used) return;
	li->countDown();
	used = true;
}

void LoaderCallback::fail() const {
	if(used) return;
	li->countDown();
	used = true;
}

Loader::Loader(std::string const &listName): listName{listName} {}

std::string const &Loader::getListName() const {
	return listName;
}
