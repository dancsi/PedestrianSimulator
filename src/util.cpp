#include "util.h"

#include <random>


namespace util
{
	std::mt19937 random_engine;
	void init()
	{
		time_t current_time;
		time(&current_time);
		char buf[30];
		sprintf(buf, "logs\\%u.log", current_time);
		//freopen(buf, "w", stderr);
		//freopen("log.txt", "w", stderr);
	}
	float rand_range(float low, float high)
	{
		std::uniform_real_distribution<> dist(low, high);
		return dist(random_engine);
	}
}
