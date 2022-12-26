#include "akila/engine/tools/string_tools.hpp"

using namespace akila;

void StringTools::leftTrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

void StringTools::rightTrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

void StringTools::trim(std::string &s) {
	leftTrim(s);
	rightTrim(s);
}
