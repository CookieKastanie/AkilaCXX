#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

namespace Akila {
	class FileSystem {
		private:
			static std::string root;

		public:
			static void init();
			static void setResourceFolder(const std::string &name);
			static std::string path(const std::string &file);
			static bool exist(const std::string &name);
	};
}
