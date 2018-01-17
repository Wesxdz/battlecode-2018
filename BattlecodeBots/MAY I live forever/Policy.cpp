#include "Policy.h"
#include <iostream>
#include "Log.h"

Policy::Policy(std::string name)
{
	this->name = name;
}

Policy::~Policy()
{
}

bool Policy::Command(units::Unit unit)
{
	bool successful = Execute(unit);
	std::cout << "Chose policy " << name << ", was successful " << successful << "\n";
	CHECK_ERRORS();
	return successful;
}
