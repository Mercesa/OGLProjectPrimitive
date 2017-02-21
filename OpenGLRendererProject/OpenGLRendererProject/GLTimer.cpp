#include "GLTimer.h"

#include <GL\glew.h>
#include <iostream>

using namespace Primitive;

// When creating a GL timer you implicitly generate two queries on construction
GLTimer::GLTimer() : mStartTime(0), mStopTime(0), mElapsedTime(0)
{
	glGenQueries(2, mQueryIDs);
}

GLTimer::~GLTimer()
{
	// Delete queries when GLTimer is out of scope
	glDeleteQueries(_countof(mQueryIDs), mQueryIDs);
}

// Start query
void GLTimer::Start() 
{
	// Use first query ID to get current timestamp
	glQueryCounter(mQueryIDs[0], GL_TIMESTAMP);
}

// Stop query
void GLTimer::Stop() 
{
	glQueryCounter(mQueryIDs[1], GL_TIMESTAMP);

	// Halt application to get timer availability (sync gpu and cpu)
	GLint stopTimerAvailable = 0;
	while (!stopTimerAvailable)
	{
		glGetQueryObjectiv(mQueryIDs[1], GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
	}

	glGetQueryObjectui64v(mQueryIDs[0], GL_QUERY_RESULT, &mStartTime);
	glGetQueryObjectui64v(mQueryIDs[1], GL_QUERY_RESULT, &mStopTime);

	mElapsedTime = (mStopTime - mStartTime) / 1000000.0;

}

// Query the elapsed time
double_t GLTimer::GetElapsedTime()
{
	return mElapsedTime;
}