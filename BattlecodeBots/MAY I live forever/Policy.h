#ifndef POLICY_H
#define POLICY_H

#include <functional>
#include <memory>
#include <string>
#include "Unit.h"
#include <map>

class Policy
{
public:
	std::string name;
public:
	Policy(std::string name);
	~Policy();
	float DebugEvaluate(bc_Unit* unit);
	 bool DebugExecute(bc_Unit* unit);
	/*
	Unit of value is Karbonite per turn
	@return The value of this policy, the highest value policy should be chosen
	*/
	std::function<float(bc_Unit*)> Evaluate;
	/*
	@execute Act out this policy in the world
	@return Was successful
	*/
	std::function<bool(bc_Unit*)> Execute;

	static std::map<std::string, double> times;

};

#endif

