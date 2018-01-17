#ifndef BUILDEROVERLORD_H
#define BUILDEROVERLORD_H

#include "bc.h"
#include <vector>
#include "Utility.h"


class BuilderOverlord
{
public:
	void Update(uint32_t round);

	void AddToUnitContainers(bc_UnitType unitType, int unitID);

	std::vector<int> m_workerIDs;
	std::vector<int> m_factoryIDs;
	std::vector<int> m_rocketIDs;

	std::vector<int> m_CombatShippingIDs;

	uint32_t m_workerCount = 0;
	uint32_t m_desiredWorkers = 4;

	uint32_t m_factoryCount = 0;
	uint32_t m_desiredFactories = 0;
	bool m_unfinishedFactories = false;

	uint32_t m_rocketCount = 0;
	uint32_t m_desiredRockets = 0;
	bool m_unfinishedRockets = false;


};

#endif