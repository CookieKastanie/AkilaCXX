#include "akila/core/resources/file_system.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace akila;

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

std::string FileSystem::exePath;
std::string FileSystem::rootFolder;

std::string FileSystem::resourcesFolder;
std::string FileSystem::userDataFolder;

void FileSystem::init() {
	exePath = procPath();

	rootFolder = "";
	resourcesFolder = "";
	userDataFolder = "";
}

void FileSystem::setRootFolder(std::string const &path) {
	rootFolder = path + "/";
}

void FileSystem::setResourcesFolder(std::string const &path) {
	resourcesFolder = path + "/";
}

void FileSystem::setUserDataFolder(std::string const &path) {
	userDataFolder = path + "/";
}

std::string FileSystem::root(std::string const &filePath) {
	return exePath + rootFolder + filePath;
}

std::string FileSystem::resources(std::string const &filePath) {
	return exePath + rootFolder + resourcesFolder + filePath;
}

std::string FileSystem::userData(std::string const &filePath) {
	return exePath + rootFolder + userDataFolder + filePath;
}

bool FileSystem::exist(std::string const &path) {
	std::ifstream f(path);
	return f.good();
}

bool FileSystem::createFolder(std::string const &path) {
	bool success = false;

	try {
		std::filesystem::create_directories(path);
		success = true;
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return success;
}

