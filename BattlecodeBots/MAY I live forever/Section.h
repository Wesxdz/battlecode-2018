#ifndef SECTION_H
#define SECTION_H

#include "bc.h"
#include <vector>
#include <list>
#include <memory>
#include "MapLocation.h"
#include <map>

/*
A group of bc_MapLocations that are connected in some way
There is always a way to pathfind between locations in the same section
Units should only care about movement within their section
*/

enum StartStatus { // These are used to evaluate Earth sections at the BEGINNING of the game
	Team, // Only your team spawns here
	Enemy, // Only the enemy team spawns here
	Mixed,  // Both teams spawn here
	None // Nobody starts in this section, we should ignore it
};

class Section
{
public:
	~Section();
	std::vector<bc_MapLocation*> locations;
	StartStatus status;
	void Add(Section* section);
	std::list<MapLocation> karboniteDeposits;

	static std::list<Section*> GenSections(std::vector<bc_MapLocation*>& passables, bc_PlanetMap* planetMap);
	static void FindEarthSectionsStatus(); // Call after generating PlayerData spawnLocations!
	static std::list<Section*> marsSections;
	static std::list<Section*> earthSections;
	static std::map<int, Section*> sectionMap; // Only contains passable locations!!!
	static int Key(MapLocation location);
	static bool InSame(MapLocation& a, MapLocation& b);
};

// A deposit can also contain unpassable terrain, but it must be connected to passable terrian with
class Deposit : public Section
{
public:
	bc_MapLocation* landing = nullptr;
	int karbonite;
	static std::list<std::shared_ptr<Deposit>> GenDeposits(std::vector<bc_MapLocation*>& locations); // TODO Pass in initial karbonite locations instead of every location
	static std::list<std::shared_ptr<Deposit>> earthDeposits;
	static std::list<std::shared_ptr<Deposit>> marsDeposits;
};

#endif

