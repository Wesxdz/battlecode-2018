#ifndef UNIT_POLICIES_H
#define UNIT_POLICIES

#include "bc.h"
#include <map>
#include <list>
#include <memory>

class Policy;

class UnitPolicies
{
public:
	std::map<bc_UnitType, std::list<std::shared_ptr<Policy>>> policies;
	void Init();
	void SetActive(bc_Unit* unit);
private:
	bc_Unit* activeUnit = nullptr;
	uint32_t activeId;
	bc_Location* activeLocation = nullptr;
	bc_MapLocation* activeMapLocation = nullptr;

	bc_Unit* storeUnit;
	bc_Direction storeDirection;
	uint32_t storeId;
};

#endif

