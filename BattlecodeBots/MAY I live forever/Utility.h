#ifndef UTILITY_H
#define UTILITY_H

#include "bc.h"
#include "MapLocation.h"
#include <array>
//#include "GameInfo.h"

namespace Utility{

	static std::array<int, 5> tryRotate;

	bc_Team GetOtherTeam(bc_Team other);

	bc_Planet GetOtherPlanet(bc_Planet);

	bool IsRobot(bc_UnitType type);

	
	static bool IsSpotOpen(int x, int y, bool mars);

	//MapLocation GetRandomPosition(GameInfo* info, MapLocation* temp);

}
#endif // !UTILITY_H

