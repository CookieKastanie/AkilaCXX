#pragma once

#include <string>
#include <sstream>
#include <fstream>

namespace akila {
	class FileSystem {
	public:
		static void setRootFolder(std::string const &path);
		static void setResourcesFolder(std::string const &path);
		static void setUserDataFolder(std::string const &path);

		static std::string root(std::string const &filePath = "");
		static std::string resources(std::string const &filePath = "");
		static std::string userData(std::string const &filePath = "");

		static bool exist(std::string const &path);
		static bool createFolder(std::string const &path);

	private:
		friend class Core;

		static std::string exePath;
		static std::string rootFolder;

		static std::string resourcesFolder;
		static std::string userDataFolder;

		static void init();
	};
}
