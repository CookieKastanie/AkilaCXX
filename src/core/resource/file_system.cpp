#include "akila/core/resource/file_system.hpp"

#include <iostream>
#include <fstream>

using namespace akila;

std::string FileSystem::root;


/////////////////
#if WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

std::string procPath() {
	char buffer[MAX_PATH];

#if WIN32
	GetModuleFileName(NULL, buffer, MAX_PATH);

	std::size_t i = MAX_PATH - 1;
	while(i && buffer[i] != '\\') {
		buffer[i--] = 0;
	}

	while(i) {
		if(buffer[i] == '\\') {
			buffer[i] = '/';
		}
		--i;
	}
	
	return std::string(buffer);
#else
	// ???
	readlink("/proc/self/exe", buffer, MAX_PATH);
	return std::string(buffer);
#endif

}
/////////////////

void FileSystem::init() {
	root = procPath();
}

void FileSystem::setRootFolder(std::string const &path) {
	root = procPath() + path + "/";
}

std::string FileSystem::path(std::string const &filePath) {
	return root + filePath;
}

bool FileSystem::exist(std::string const &filePath) {
	std::ifstream f((root + filePath).c_str());
	return f.good();
}
