#ifndef UTILITY_H
#define UTILITY_H

#include "bc.h"
#include "MapLocation.h"
//#include "GameInfo.h"

namespace Utility{

	bc_Team GetOtherTeam(bc_Team other);

	bc_Planet GetOtherPlanet(bc_Planet);

	bool IsRobot(bc_UnitType type);

	//MapLocation GetRandomPosition(GameInfo* info, MapLocation* temp);

}
#endif // !UTILITY_H
