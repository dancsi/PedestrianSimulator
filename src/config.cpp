#include "config.h"

#include <windows.h>
#include <string>

namespace config
{
	std::string fname;
	void read(char* file)
	{
		fname = std::string(".\\")+std::string(file);
	}

	template<typename T>
	T get(std::string section, std::string key)
	{
		return T();
	}

	template<>
	int get<int>(std::string section, std::string key)
	{
		return GetPrivateProfileInt(section.c_str(), key.c_str(), 0, fname.c_str());
	}

	template<>
	double get<double>(std::string section, std::string key)
	{
		char buf[100];
		GetPrivateProfileString(section.c_str(), key.c_str(), "0.0", buf, 100, fname.c_str());
		return std::stof(buf);
	}
	template<>
	std::string get<std::string>(std::string section, std::string key)
	{
		char buf[100];
		GetPrivateProfileString(section.c_str(), key.c_str(), "", buf, 100, fname.c_str());
		return std::string(buf);
	}
}