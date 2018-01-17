#include "Policy.h"
#include <iostream>
#include "Log.h"
#include "GameController.h"

Policy::Policy(std::string name)
{
	this->name = name;
}

Policy::Policy(std::string name, std::function<float(units::Unit)> evaluate, std::function<bool(units::Unit)> execute)
{
	this->name = name;
	Evaluate = evaluate;
	Execute = execute;
}

Policy::~Policy()
{
}

bool Policy::Command(units::Unit unit)
{
	bool successful = Execute(unit);
	std::cout << "r:" << GameController::Round() << " p:" <<  name << " s:" << successful << "\n";
	CHECK_ERRORS();
	return successful;
}
