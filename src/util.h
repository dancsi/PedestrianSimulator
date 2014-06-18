#pragma once

#include <ctime>
#include <cstdlib>
#include <cstdio>

#define LOG(format, ...) fprintf(stderr, "%s(%d): " format, __FILE__, __LINE__, __VA_ARGS__);

namespace util
{
	void init();
}