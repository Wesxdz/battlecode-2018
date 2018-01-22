#ifndef SECTION_H
#define SECTION_H

#include "bc.h"
#include <vector>
#include <list>
#include <memory>
#include "MapLocation.h"

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
	static std::list<std::shared_ptr<Section>> GenSections(std::vector<bc_MapLocation*>& passables);
	static void FindEarthSectionsStatus(); // Call after generating PlayerData spawnLocations!
	static std::list<std::shared_ptr<Section>> marsSections;
	static std::list<std::shared_ptr<Section>> earthSections;
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

