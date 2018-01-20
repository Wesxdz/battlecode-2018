#ifndef UTILITY_H
#define UTILITY_H

#include "bc.h"
#include "MapLocation.h"
#include <array>
//#include "GameInfo.h"

namespace Utility{



	bc_Team GetOtherTeam(bc_Team other);

	bc_Planet GetOtherPlanet(bc_Planet);

	bool IsRobot(bc_UnitType type);

	bool IsAttackRobot(bc_UnitType type);

	bc_Direction DirectionTo(int sourceX, int sourceY, int destX, int destY);

	

	//MapLocation GetRandomPosition(GameInfo* info, MapLocation* temp);

}
#endif // !UTILITY_H

