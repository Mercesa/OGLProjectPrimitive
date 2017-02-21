#pragma once

#include <cstdio>
#include <stdint.h>
#include <math.h>

namespace Primitive
{


class GLTimer
{
public:
	GLTimer();
	~GLTimer();

	void Start();
	void Stop();
	double_t GetElapsedTime();

private:

	uint64_t mStartTime;
	uint64_t mStopTime;
	unsigned int mQueryIDs[2];

	double_t mElapsedTime;
};

}