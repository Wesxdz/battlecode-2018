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
	/*
	Manages factory production, worker replication, and structure blueprinting
	*/
	static std::map<uint16_t, std::vector<uint16_t>> buildProjects; // Structure ids, workers building ids
	static std::map<uint16_t, std::vector<uint16_t>> rockets; // Rocket ID, units heading towards it
	static std::map<uint16_t, MapLocation> seekKarbonite;
	static std::list<std::shared_ptr<Deposit>> sortedLandings;
	static std::vector<bc_UnitType> rocketLoadType;
};

#endif