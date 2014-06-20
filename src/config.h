#pragma once
#include <string>

namespace config
{
	void read(char* file);
	template<typename T>
	T get(std::string section, std::string key);
}

