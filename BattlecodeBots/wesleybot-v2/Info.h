#ifndef INFO_H
#define INFO_H

#include "bc.h"
#include <map>

class Info
{
public:
	Info();
	~Info();
	void Update();
	static std::map<bc_UnitType, int> numTeamUnits;
	static std::map<bc_UnitType, int> numEnemyUnits;
};

#endif

