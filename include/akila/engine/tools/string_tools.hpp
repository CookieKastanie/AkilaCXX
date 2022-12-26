#pragma once

#include <string>

namespace akila {
	class StringTools {
		static void leftTrim(std::string &s);
		static void rightTrim(std::string &s);
		static void trim(std::string &s);
	};
}
