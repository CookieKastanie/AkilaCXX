#include "Akila/files/FileSystem.hpp"

#include <iostream>
#include <fstream>

using namespace Akila;

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

	unsigned int i = strlen(buffer);
	while(i && buffer[i] != '\\') {
		buffer[i--] = 0;
	}

	while(i) {
		if(buffer[i] == '\\') buffer[i] = '/';
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

std::string FileSystem::path(const std::string &file) {
	return root + file;
}



bool FileSystem::exist(const std::string &name) {
	std::ifstream f((root + name).c_str());
	return f.good();
}
