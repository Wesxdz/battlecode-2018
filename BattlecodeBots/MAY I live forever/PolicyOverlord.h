#ifndef POLICY_OVERLORD_H
#define POLICY_OVERLORD_H

#include "bc.h"
#include <map>
#include <vector>
#include <memory>
#include "Unit.h"

class Policy;

class PolicyOverlord
{
public:
	std::map<bc_UnitType, std::vector<std::shared_ptr<Policy>>> policies;
	PolicyOverlord();
	void Update();
	
	static bc_Direction storeDirection;
	static MapLocation storeLocation;
	static uint16_t storeId;
	static bc_UnitType storeUnitType;

	static bc_UnitType HighestPriority();
};

#endif

