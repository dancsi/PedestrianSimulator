#pragma once

#include <ctime>
#include <cstdlib>
#include <cstdio>

#define LOG(format, ...) do {fprintf(stderr, "%s(%d): " format, __FILE__, __LINE__, __VA_ARGS__); fprintf(stderr, "\n");} while(0)

namespace util
{
	void init();
}