#ifndef POLICY_H
#define POLICY_H

#include <functional>
#include <memory>
#include <string>
#include "Unit.h"

class Policy
{
public:
	std::string name;
public:
	Policy(std::string name);
	Policy(std::string name, std::function<float(units::Unit)> evaluate, std::function<bool(units::Unit)> execute);
	~Policy();
	 bool Command(units::Unit);
	/*
	Unit of value is Karbonite per turn
	@return The value of this policy, the highest value policy should be chosen
	*/
	std::function<float(units::Unit)> Evaluate;
	/*
	@execute Act out this policy in the world
	@return Was successful
	*/
	std::function<bool(units::Unit)> Execute;

};

#endif

