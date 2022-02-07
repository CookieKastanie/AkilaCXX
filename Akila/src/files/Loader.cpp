#include "Akila/core/Core.hpp"
#include "Akila/files/Loader.hpp"
#include "Akila/files/FileSystem.hpp"
#include "stbimage/stb_image.h"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

/*/
void Loader::splitString(std::vector<std::string> &list, std::string &str, const std::string &delimiter) {
	list.clear();

	std::size_t pos = 0;
	while((pos = str.find(delimiter)) != std::string::npos) {
		list.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}

	list.push_back(str);
}

void Loader::trimString(std::string &str) {
	std::size_t i = 0;

	while((i < str.size()) && std::isspace(str[i])) ++i;
	str.erase(0, i);

	if(str.size() == 0) return;

	i = str.size() - 1;

	while((i >= 0) && std::isspace(str[i])) --i;
	str.erase(i + 1, str.size());
}
//*/
