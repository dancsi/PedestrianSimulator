#include "util.h"

namespace util
{
	void init()
	{
		time_t current_time;
		time(&current_time);
		char buf[30];
		sprintf(buf, "logs\\%u.log", current_time);
		//freopen(buf, "w", stderr);
		//freopen("log.txt", "w", stderr);
	}
}
