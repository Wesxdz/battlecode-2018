#ifndef INFO_H
#define INFO_H

#include "bc.h"
#include <map>
#include "Section.h"

class Info
{
public:
	static std::map<bc_UnitType, int> numTeamUnits;
	static std::map<bc_UnitType, int> numEnemyUnits;
};

#endif

