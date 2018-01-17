#ifndef BUILDEROVERLORD_H
#define BUILDEROVERLORD_H

#include "bc.h"
#include "PlayerData.h"
#include <vector>
#include "Utility.h"





class BuilderOverlord
{
public:
	void Update(uint32_t round);

	std::vector<units::Unit*> m_combatShippingIDs;

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