#pragma once

#include <string>

namespace akila {
	class StringTools {
	public:
		static void leftTrim(std::string &s);
		static void rightTrim(std::string &s);
		static void trim(std::string &s);
		static void replaceAll(std::string &s, std::string const &search, std::string const &replace);
	};
}
