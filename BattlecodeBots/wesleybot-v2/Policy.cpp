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

void Policy::Command()
{
	//std::cout << "Executing policy choice " << name << "\n";
	Execute(0);
	//CHECK_ERRORS();
}
