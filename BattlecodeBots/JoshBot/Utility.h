#ifndef UTILITY_H
#define UTILITY_H

#include "bc.h"
#include "GameInfo.h"

namespace Utility{

	bool CheckError();

	bc_Team GetOtherTeam(bc_Team other);

	bool IsRobot(bc_UnitType type);

	MapLocation GetRandomPosition(GameInfo* info, MapLocation* temp);

}
#endif // !UTILITY_H

