#include "Clock.h"
typedef std::chrono::time_point<std::chrono::system_clock> time_point;
time_point Clock::now()
{
	return std::chrono::system_clock::now();
}


