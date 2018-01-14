#ifndef POLICY_H
#define POLICY_H

#include <functional>
#include <memory>
#include <string>

class Policy
{
public:
	std::string name;
public:
	Policy(std::string name);
	~Policy();
	void Command();
	/*
	@execute Act out this policy in the world
	@return Was successful
	*/
	std::function<bool(float)> Execute;
	/*
	Unit of value is Karbonite per turn
	@return The value of this policy, the highest value policy should be chosen
	*/
	std::function<float()> Evaluate;
	///*
	//@return Whether this policy can be used with another policy
	//*/
	//std::function<bool(std::shared_ptr<Policy>)> IsCohesive;
	///*
	//@return How much value this policy adds to another policy when used together
	//*/
	//std::function<float(std::shared_ptr<Policy>)> Affect;

};

#endif

