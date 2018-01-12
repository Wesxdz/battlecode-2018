#ifndef POLICY_H
#define POLICY_H

#include <functional>

class Policy
{
public:
	Policy();
	~Policy();
	/*
	@execute Do everything this policy 
	*/
	std::function<void()> Execute;
	/*
	@return The value of this policy, the highest value policy should be chosen
	*/
	std::function<int()> Evaluate;
};

#endif

