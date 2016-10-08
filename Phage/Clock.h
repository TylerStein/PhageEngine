#pragma once
#include <chrono>
class Clock {
public:
	//typedef std::chrono::time_point<std::chrono::system_clock> time_point;

	static float now();
};
