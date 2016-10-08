#include "Clock.h"
//typedef std::chrono::time_point<std::chrono::system_clock> time_point;
float Clock::now()
{
	auto start = std::chrono::system_clock::now();

	auto stop = std::chrono::system_clock::now();

	using floatMillisecs = std::chrono::duration<float, std::chrono::milliseconds::period>;

	static_assert(std::chrono::treat_as_floating_point<floatMillisecs::rep>::value, "Rep required to be floating point");

	auto float_time = floatMillisecs(stop - start);

	return float_time.count();
}


