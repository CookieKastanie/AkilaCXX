#pragma once

#include <string>
#include <sstream>
#include <fstream>

namespace akila {
	class FileSystem {
	public:
		static void setRootFolder(std::string const &path);
		static std::string path(std::string const &filePath);
		static bool exist(std::string const &filePath);

	private:
		friend class Core;

		static std::string root;

		static void init();
	};
}
