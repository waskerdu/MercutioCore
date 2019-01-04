#include "stdafx.h"
#include "meClock.h"

double Clock::Time()
{
	return (double)(clock() - t) / CLOCKS_PER_SEC;
}

Clock::Clock()
{
	t = clock();
}


Clock::~Clock()
{
}
