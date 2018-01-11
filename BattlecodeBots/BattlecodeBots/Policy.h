#pragma once

#include <functional>

class Policy
{
public:
	std::function<float()> evaluateStart;
	std::function<float()> evaluateTurn;
	std::function<void()> act;
public:
	Policy();
	~Policy();
};

