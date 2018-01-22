#include "Policy.h"
#include <iostream>
#include "Log.h"
#include "GameController.h"
#include <chrono>

std::map<std::string, double> Policy::times;

Policy::Policy(std::string name)
{
	this->name = name;
}

Policy::~Policy()
{
}

float Policy::DebugEvaluate(bc_Unit* unit)
{
	auto start = std::chrono::system_clock::now();
	//std::cout << "Evaluating " << " p:" << name << std::endl;
	float score = Evaluate(unit);
	CHECK_ERRORS();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> time = end - start;
	times[name] += time.count();
	return score;
}

bool Policy::DebugExecute(bc_Unit* unit)
{
	std::cout << "r:" << GameController::Round() << " p:" << name << std::endl;
	bool successful = Execute(unit);
	CHECK_ERRORS();
	std::cout << "s:" << successful << std::endl;
	return successful;
}
