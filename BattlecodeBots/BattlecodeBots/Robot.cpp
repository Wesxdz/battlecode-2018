#include "Robot.h"



Robot::Robot()
{
}


Robot::~Robot()
{
}

uint32_t Robot::Cost()
{
	return bc_UnitType_factory_cost(type);
}
