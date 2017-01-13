#pragma once

#include "stdafx.h"

namespace Primitive{

class EngineTimer
{
public:
	EngineTimer();
	~EngineTimer();

	void Start();
	void Update();
	float_t GetDeltaTime();
	float_t GetTotalTime();
};

}
