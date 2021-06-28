#pragma once

#include <string>

namespace Akila {
	class FileSystem {
		private:
			static std::string root;

		public:
			static void init();
			static std::string path(const std::string &file);
			static bool exist(const std::string &name);
	};
}
