#include <iostream>
#include <memory>

#include "bc.h"

#include "GameController.h"
#include "GameMap.h"
#include "PlanetMap.h"
#include "Research.h"
#include "OrbitPattern.h"
#include "AsteroidPattern.h"
#include "TeamArray.h"
#include "Log.h"

#include "Unit.h"
#include "Worker.h"
#include "MapLocation.h"
#include "Location.h"

GameController gc;
GameMap gameMap;

Research research;
OrbitPattern orbitPattern;
AsteroidPattern asteroidPattern;
TeamArray teamArray;


bc_Team thisTeam = gc.Team();
bc_Planet thisPlanet = gc.Planet();
PlanetMap* thisPlanetMap = gameMap.OurPlanet();

// Everything Static should be in GameController?

/*
Instead of micromanaging individual unit behavior, prioritize goals based on available data and
then determine best execution strategy
*/
int main()
{

	srand(0);

	std::cout << "C++ Bot initialize" << std::endl;
	//GameController player;
	//PlanetMap map;
	//map.self = player.PlanetMap();
	//MapLocation test = MapLocation(player.Planet(), map.Width() / 2, map.Height() / 2);
	while (true)
	{
		uint32_t round = gc.Round();
		std::cout << "Round: " << round << std::endl;
		CHECK_ERRORS();
		//auto units = player.Units(bc_Selection::MyTeam);
		//for (auto unit : units) {
		//	if (unit.type == bc_UnitType::Worker) {
		//		auto worker = std::make_shared<units::Worker>();
		//		worker.Init(unit.id);
		//		Location workerLocation = worker->Loc();
		//		MapLocation onMap = workerLocation.ToMapLocation();
		//		//bc_Direction directionToMove = Southwest;
		//		bc_Direction directionToMove = Pathfinding::PickGreedy(onMap, test);
		//		if (directionToMove != bc_Direction::Center && worker->IsMoveReady()) {
		//			worker->Move(directionToMove);
		//		}
		//	}
		//}
		gc.EndTurn();
	}
}