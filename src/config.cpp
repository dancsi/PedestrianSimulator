#include "config.h"

#include <windows.h>

namespace config
{
	std::string fname;
	void read(char* file)
	{
		fname = std::string(file);
	}

	template<typename T>
	T get(std::string section, std::string key)
	{
		return T();
	}

	template<>
	int get<int>(std::string section, std::string key)
	{
		return GetPrivateProfileInt(section.c_str(), key.c_str(), 0, (std::string(".\\")+fname).c_str());
	}
}