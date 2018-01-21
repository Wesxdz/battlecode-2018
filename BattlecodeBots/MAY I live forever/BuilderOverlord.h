#ifndef BUILDEROVERLORD_H
#define BUILDEROVERLORD_H

#include "bc.h"
#include "PlayerData.h"
#include <vector>
#include "Utility.h"
#include "Worker.h"
#include "Factory.h"
#include "Rocket.h"
#include "Section.h"

class BuilderOverlord
{
public:
	BuilderOverlord();
	void Update();
	void DesireUnits();
	static std::map<uint16_t, std::vector<uint16_t>> buildProjects; // Structure ids, workers building ids
	static std::list<std::shared_ptr<Deposit>> sortedLandings;
};

#endif