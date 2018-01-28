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
#include "Pathfind.h"

struct RocketInfo {
	static std::vector<bc_UnitType> rocketLoadType; // Desired Load Types
	static std::list<Section*> marsSectionsNotVisited;
	static std::map<Section*, int> bestMarsSections;
	static int rocketsLaunched;
	static std::map<bc_UnitType, int> unitsLoaded;
};

class BuilderOverlord
{
public:
	BuilderOverlord();
	void Update();
	void DesireUnits();
	/*
	Manages factory production, worker replication, and structure blueprinting
	*/
	static void ManageProduction();

	static float ProductionScore(units::Factory& factory);
	static bc_UnitType Priority(units::Factory& factory);

	static float ReplicationScore(units::Worker& worker);

	static float FactoryPlacementScore(MapLocation location);
	static float RocketPlacementScore(MapLocation location);
	static void ChooseRocketLoad();

	static std::map<uint16_t, std::vector<uint16_t>> buildProjects; // Structure ids, workers building ids
	static std::map<uint16_t, std::vector<uint16_t>> rockets; // Rocket ID, units heading towards it
	static std::map<Section*, FlowChart> findKarbonite;
	static std::map<Section*, FlowChart> findRocket;
	static std::map<uint16_t, int> miningSuccess; // The Karbonite mined by a particular worker in the last 3 turns
	//static std::list<Section*> PrioritizeSections(std::list<Section*> sections);
	void CreateKarboniteFlows();

	
};

#endif