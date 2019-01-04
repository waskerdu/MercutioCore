#pragma once
#include <time.h>
class Clock
{
	double time = 0.0;
	clock_t t = 0;
public:
	double Time();
	Clock();
	~Clock();
};

