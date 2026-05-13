#pragma once
#include "../pch.h"
#include <chrono>

class TimeManager
{
	TimeManager();
	~TimeManager();



	int deltaTime{}, prevTime{};
};

